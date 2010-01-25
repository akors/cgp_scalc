// parsing.hpp

/*
 *   scalc - A simple calculator
 *   Copyright (C) 2009, 2010  Alexander Korsunsky
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARSING_HPP_
#define PARSING_HPP_

#include <vector>
#include <algorithm>


template <typename IteratorT>
void delete_all (IteratorT first, IteratorT last)
{
    // iterator is copied by value, so we can use the argument to iterate through the container
    for ( ; first < last ; first++)
    {
        delete *first;
        *first = NULL;
    }
}


/** Memory handler class for a specific type.
*
* This class stores pointers to objects of a specific type on the heap.  <br>
* The pointers can be added using registerNew() and removed using unregister().
* <br>
* You can also delete a registered pointer using registeredDelete(). <br>
* To delete all pointers of a type use deleteAll.  <br>
* @note All pointers passed to the member functions can be NULL. In this case no
* error occurs, but no action is performed.
*
* @tparam T The type of the data the pointers will point to
*
*/
template <typename T>
class RegisteredPtrs {
    /** all registered pointers for a type */
    std::vector<T*> currentlyRegistered;

public:

    /** Register a pointer.
    *
    * Put a pointer into the internal pointer Container.
    * If ptr is NULL, it is not added to the container.
    *
    * @param ptr The pointer that shall be added
    * @return The pointer given as argument
    * @post If the pointer was not null, it is now present in the internal
    * pointer container
    */
    T* registerNew(T* ptr);

    /** Registered delete.
    * Delete a pointer, set to NULL and if present in the pointer container,
    * remove it from there.
    *
    * @param ptr A reference to a pointer that you want to be deleted and
    * removed from the container
    *
    * @note The pointer is deleted in any case, wether or not the pointer is
    * present in the container
    *
    * @post If the pointer is not NULL, it is deleted.
    * @post If the pointer was present in the internal pointer Container,
    * it is removed.
    */
    void registeredDelete(T*& ptr);

    /** Unregister the pointer.
    *
    * The internal pointer container will be searched for the pointer.
    * If present, it will be removed from there.
    *
    * @param ptr The pointer that shall be removed from the internal
    * pointer container
    * @return The pointer given as argument
    *
    * @post The pointer is not present in the internal pointer container
    */
    T* unregister(T* ptr);

    /** delete all registered pointers
    * Iterates through the list of pointers present in the internal pointer
    * container and deletes them from memory, clears the internal pointer
    * container
    *
    * @post All pointers contained in the internal pointer container are deleted,
    * @post the internal pointer container is now empty.
    */
    void deleteAll();

    /** Destrcutor.
    * Calls deleteAll().
    */
    ~RegisteredPtrs()
    {
        deleteAll();
    }
};

template <typename T>
T* RegisteredPtrs<T>::registerNew(T* ptr)
{
    // only add if ptr is not NULL
    if ( ptr != NULL )
        currentlyRegistered.push_back(ptr);

    return ptr;
}

template <typename T>
void RegisteredPtrs<T>::registeredDelete(T*& ptr)
{
    // dont bother if NULL
    if (ptr == NULL)
        return;

    // delete and set to NULL in any case
    delete ptr;

    // unregister pointer
    unregister(ptr);

    ptr = NULL;
}

template <typename T>
T* RegisteredPtrs<T>::unregister(T* ptr)
{
    typename std::vector<T*>::iterator it;

    // find the pointer in the vector
    it = std::find(currentlyRegistered.begin(), currentlyRegistered.end(),
                    ptr);

    // erase element from the vector if found
    if ( it != currentlyRegistered.end() )
        currentlyRegistered.erase(it);

    return ptr;
}

template <typename T>
void RegisteredPtrs<T>::deleteAll()
{
    delete_all (currentlyRegistered.begin(), currentlyRegistered.begin());
    currentlyRegistered.clear();
}

extern int yyparse ();
extern void do_cleanup();
extern FILE* yyin;

#define MEMORY_ASSERT(p) \
if ( (p) == NULL ) \
    throw std::bad_alloc();


#endif // ifndef PARSING_HPP_
