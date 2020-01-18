/*
  This file is part of the SC Library, version 3.
  The SC Library provides support for parallel scientific applications.

  Copyright (C) 2019 individual authors

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

/** \file sc3_array.h
 */

#ifndef SC3_ARRAY_H
#define SC3_ARRAY_H

#include <sc3_error.h>

typedef struct sc3_array sc3_array_t;

#ifdef __cplusplus
extern              "C"
{
#if 0
}
#endif
#endif

/** Check whether an array is not NULL and internally consistent.
 * The array may be valid in both its setup and usage phases.
 * \param [in] a        Any pointer.
 * \param [out] reason  If not NULL, existing string of length SC3_BUFSIZE
 *                      is set to "" if answer is yes or reason if no.
 * \return              True iff pointer is not NULL and array consistent.
 */
int                 sc3_array_is_valid (sc3_array_t * a, char *reason);

/** Check whether an array is not NULL, consistent and not setup.
 * This means that the array is not in its usage phase.
 * \param [in] a        Any pointer.
 * \param [out] reason  If not NULL, existing string of length SC3_BUFSIZE
 *                      is set to "" if answer is yes or reason if no.
 * \return              True iff pointer not NULL, array consistent, not setup.
 */
int                 sc3_array_is_new (sc3_array_t * a, char *reason);

/** Check whether an array is not NULL, internally consistent and setup.
 * This means that the array is in its usage phase.
 * \param [in] a        Any pointer.
 * \param [out] reason  If not NULL, existing string of length SC3_BUFSIZE
 *                      is set to "" if answer is yes or reason if no.
 * \return              True iff pointer not NULL, array consistent and setup.
 */
int                 sc3_array_is_setup (sc3_array_t * a, char *reason);

/** Create a new array object in its setup phase.
 * It begins with default parameters that can be overridden explicitly.
 * Setting and modifying parameters is only allowed in the setup phase.
 * Call \ref sc3_array_setup to change the array into its usage phase.
 * After that, no more parameters may be set.
 * \param [in,out] aator    An allocator that is setup.
 *                          The allocator is refd and remembered internally
 *                          and will be unrefd on array destruction.
 * \param [out] ap      Pointer must not be NULL.
 *                      If the function returns an error, value set to NULL.
 *                      Otherwise, value set to an array with default values.
 * \return              NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_new (sc3_allocator_t * aator,
                                   sc3_array_t ** ap);

/** Set the size of each array element in bytes.
 * \param [in,out] a    The array must not be setup.
 * \param [in] esize    Element size in bytes.  Zero is legal, one the default.
 * \return              NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_set_elem_size (sc3_array_t * a, size_t esize);

/** Set the initial number of array elements.
 * \param [in,out] a    The array must not be setup.
 * \param [in] ecount   Element count on setup.  Zero is legal and default.
 * \return              NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_set_elem_count (sc3_array_t * a, int ecount);

/** Set the minimum required number of array elements to allocate on setup.
 * \param [in,out] a    The array must not be setup.
 * \param [in] ealloc   Minimum number of elements initially allocated.
 *                      Legal if this is smaller than the initial count.
 *                      Must be non-negative; default is 8.
 * \return              NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_set_elem_alloc (sc3_array_t * a, int ealloc);

/** Set the resizable property of an array.
 * It determines whether the array may be resized after setup.
 * \param [in,out] a        The array must not be setup.
 * \param [in] resizable    Boolean; default is true.
 * \return                  NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_set_resizable (sc3_array_t * a, int resizable);

/** Set the initzero property of an array.
 * If set to true, the array memory is zeroed during setup.
 * \param [in,out] a        The array must not be setup.
 * \param [in] initzero     Boolean; default is false.
 * \return                  NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_set_initzero (sc3_array_t * a, int initzero);

/** Setup an array and put it into its usable phase.
 * \param [in,out] a    This array must not yet be setup.
 *                      Internal storage is allocated, the setup phase ends,
 *                      and the array is put into its usable phase.
 * \return              NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_setup (sc3_array_t * a);

/** Increase the reference count on an array by 1.
 * This is only allowed after the array has been setup.
 * \param [in,out] a    This array must be setup.  Its refcount is increased.
 * \return              NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_ref (sc3_array_t * a);

/** Decrease the reference count on an array by 1.
 * If the reference count drops to zero, the allocator is deallocated.
 * \param [in,out] ap   The pointer must not be NULL and the array valid.
 *                      Its refcount is decreased.  If it reaches zero,
 *                      the array is destroyed and the value set to NULL.
 * \return              NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_unref (sc3_array_t ** ap);

/** Destroy an array with a reference count of 1.
 * It is an error to destroy an array that is multiply refd.
 * \param [in,out] ap   This array must be valid and have a refcount of 1.
 *                      On output, value is set to NULL.
 * \return              NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_destroy (sc3_array_t ** ap);

/** Index an array element.
 * \param [in] a        The array must be setup.
 * \param [in] i        Index must be in [0, element count).
 * \param [out] pe      Address of array element at index \b i.
 * \return              NULL on success, error object otherwise.
 */
sc3_error_t        *sc3_array_index (sc3_array_t * a, int i, void **p);

#ifdef __cplusplus
#if 0
{
#endif
}
#endif

#endif /* !SC3_ARRAY_H */
