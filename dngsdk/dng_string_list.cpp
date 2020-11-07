/*****************************************************************************/
// Copyright 2006-2019 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

#include "dng_string_list.h"

#include "dng_bottlenecks.h"
#include "dng_exceptions.h"
#include "dng_string.h"
#include "dng_utils.h"
#if GPR_WRITING || GPR_READING
#include "gpr_allocator.h"
#endif

/*****************************************************************************/

dng_string_list::dng_string_list ()

	:	fCount     (0)
	,	fAllocated (0)
	,	fList      (nullptr)

	{

	}

/*****************************************************************************/

dng_string_list::~dng_string_list ()
	{

	Clear ();

	}

/*****************************************************************************/

void dng_string_list::Allocate (uint32 minSize)
	{

	if (fAllocated < minSize)
		{

		uint32 newSize = Max_uint32 (minSize, fAllocated * 2);

#if GPR_WRITING || GPR_READING
		dng_string **list = (dng_string **) gpr_global_malloc (newSize * sizeof (dng_string *));
#else
		dng_string **list = (dng_string **) malloc (newSize * sizeof (dng_string *));
#endif

		if (!list)
			{

			ThrowMemoryFull ();

			}

		if (fCount)
			{

			memcpy (list, fList, fCount * (uint32) sizeof (dng_string *));

			}

		if (fList)
			{

#if GPR_WRITING || GPR_READING
			gpr_global_free (fList);
#else
			free (fList);
#endif

			}

		fList = list;

		fAllocated = newSize;

		}

	}

/*****************************************************************************/

void dng_string_list::Insert (uint32 index,
							  const dng_string &s)
	{

	Allocate (fCount + 1);

	dng_string *ss = new dng_string (s);

	if (!ss)
		{

		ThrowMemoryFull ();

		}

	fCount++;

	for (uint32 j = fCount - 1; j > index; j--)
		{

		fList [j] = fList [j - 1];

		}

	fList [index] = ss;

	}

/*****************************************************************************/

bool dng_string_list::Contains (const dng_string &s) const
	{

	for (uint32 j = 0; j < fCount; j++)
		{

		if ((*this) [j] == s)
			{

			return true;

			}

		}

	return false;

	}

/*****************************************************************************/

void dng_string_list::Clear ()
	{

	if (fList)
		{

		for (uint32 index = 0; index < fCount; index++)
			{

			delete fList [index];

			}

#if GPR_WRITING || GPR_READING
		gpr_global_free (fList);
#else
		free (fList);
#endif

		fList = nullptr;

		}

	fCount     = 0;
	fAllocated = 0;

	}

/*****************************************************************************/
