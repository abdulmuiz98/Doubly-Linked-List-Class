#ifndef dslist_h_
#define dslist_h_

#include <cassert>

// --------------------------------------------------------
// NODE CLASS

template <class T>
class Node {
  public:
    Node () : next_(NULL), prev_(NULL) {}

    //  member variables
    T value;
    Node<T>* prev_;
    Node<T>* next_;
};


// forward declaration of ds list b/s it is referenced in list_iterator
template <class T> class dlist;
// --------------------------------------------------------
// LIST ITERATOR CLASS
template <class T>
class list_iterator {
  private:
    // representation
    Node<T>* ptr_;

  public:
    list_iterator () : ptr_(NULL) {}
    list_iterator ( Node<T>* p ) : prt_(p) {}
    list_iterator ( const list_iterator<T>& old ) : ptr_(old.ptr_) {}

    // operator overloading
    // ASSIGNMENT OPERATOR
    list_iterator<T>& operator= (const list_iterator<T>& old)
      { this->ptr_ = old.ptr_; return *this; }
    // DEREFERENCE OPERATOR - return what ptr_ points at
    T& operator* () { return ptr_->value; }
    // INCREMENT & DECREMENT OPERATORS
    list_iterator<T>& operator++ () { // ++iter
      // increment iterator, then return new state
      ptr_ = ptr_->next_;
      return *this;
    }
    list_iterator<T>& operator++ (int) { // iter++
      // return current state of iterator, then increment it
      list_iterator<T>& temp(*this);
      ptr_ = ptr_->next;
      return temp;
    }
    list_iterator<T>& operator-- () { // --iter
      // decrement iterator, then return new state
      pre_ = pre_->prev_;
      return *this;
    }
    list_iterator<T>& operator-- (int) { // iter--
      // return current iterator state, then decrement it
      list_iterator<T>& temp(*this);
      ptr_ = ptr_->prev_;
      return temp;
    }

    // dslist class needs to access list_iterator's ptr_ private variable
    // so, we make it friends with list_iterator<T> class
    friend class dslist<T>;

    // COMPARISON OPERATOR
    bool operator== (const list_iterator<T>& r) const {
      return this->ptr_ == r.ptr_;
    }
    bool operator!= (const list_iterator<T>& r) const {
      return this->ptr_ != r.ptr_;
    }

};


// --------------------------------------------------------
// DS LIST CLASS IMPLEMENTATION
template <class T>
class dslist {
  private:
    // private helper functions
    /*TODO*/ void copy_list (const dslist<T>& old);
    /*TODO*/ void destory_list ();

    // REPRESENTATION
    Node<T>* head_;
    Node<T>* tail_;
    unsigned int size_;

  public:
    // default constructor, assignment constructor, copy constructor,
    // & deconstructor
    dslist () : head_(0), tail_(0), size_(0) {} // 0 for NULL
    dslist (const dslist<T>& old) : { this->copy_list(old); }
    /*TODO -- DONE*/ dslist<T>& operator= (const dslist<T>& old);
    ~dslist () { this->destory_list(); }

    // SIMPLE ACCESSORS
    unsigned int size () const { return size_; }
    bool empty () const { return head_==0; }

    // SIMPLE MODIFIERS
    void clear () { this->destory_list(); }

    // read / write access to contents
    const T& front () const { return head_->value_; }
    T& front () { return head_->value_; }
    const T& back () const { return tail_->value_; }
    T& back () { return tail_->value; }

    // modify linked list structure
    /*TODO -- DONE*/ void push_front (const T& v);
    /*TODO -- DONE*/ void pop_front ();
    /*TODO -- DONE*/ void push_back (const T& v);
    /*TODO -- DONE*/ void pop_back ();

    // iterator modifier functions
    typedef list_iterator<T> iterator;
    /*TODO -- DONE*/ iterator erase (iterator itr);
    /*TODO*/ iterator insert (iterator itr, cosnt T& v);
    iterator begin () { return iterator(head_); }
    iterator end () { return iterator(0); }
};


// IMPLEMENTATIONS !!!
template <class T>
dslist<T>& dslist<T>::operator= (const dslist<T>& old) {
  // check for self assignment
  if ( &old != this ) {
    this->destory_list();
    this->copy_list(old);
  }

  return *this;
}

template <class T>
void dslist<T>::push_front (const T& v) {
  // add v to the front -- relinking
  list_iterator<T> next_itr = begin ();
  list_iterator<T> first_itr = next_itr++; // itr's old statr is returned and next_itr increments

  // make a new node for v
  Node<T>* v_node;
  v_node = new Node<T>;
  v_node->value = T;

  // make first_itr's ptr_->next_ point to v -- and vice versa
  first_itr.ptr_->next_ = v_node;
  v_node->prev_ = first_itr.ptr_;

  // make next_itr's ptr_->prev_ point to v -- and vice versa
  next_itr.ptr->prev_ = v_node;
  v_node->next_ = next_itr.ptr_;

  ++size_;
}

template <class T>
void dslist<T>::pop_front () {
  // remove the item in the front
  Node<T>* to_remove = this->head_;
  this->head_ = ++ (this->begin());

  to_remove->next_ = 0;
  head_->prev_ = 0;

  delete to_remove;
  --size;
}

template <class T>
void dslist<T>::push_back (const T& v) {
  // add v to the end of the list
  Node<T>* to_add = new Node<T>;
  to_add->value_ = v; // set the node value to v (object to add to list)
  to_add->prev_ = tail_; // make the end of the list point forward (next_) to v
  tail_->next_ = to_add; // make v point back to end of list (prev_)

  tail_ = to_add; // now, make to_add the new end of list by assigning it to tail_

  ++size_; // inc size
}

template <class T>
void dslist<T>::pop_back () {
  // remove the item at the end of the list
  Node<T>* to_remove = tail_;
  tail_ = tail_->prev_;
  tail_->next_ = 0;
  to_remove->prev_ = 0;

  delete to_remove;
  --size_;
}

template <class T>
iterator dslist<T>::erase (iterator itr) {
  // remove the object that the iterator is pointing at in this list.
  Node<T>* to_left = itr.ptr_->prev_;
  Node<T>* to_right = itr.ptr_->next_;

  itr.ptr_->next_ = 0;
  itr.ptr_->prev_ = 0;

  // do not need to delete itr b/c not dynamically allocated

  // make sure the Node pointers are not null before accessing properties of it1
  if (to_left != 0) to_left->next_ = to_right;
  if (to_right != 0) to_right->prev_ = to_left;

  return iterator(to_right);
}

template <class T>
iterator dslist<T>::insert (iterator itr, const T& v) {
  // insert v after the Node that itr's ptr_ points to
  Node<T>* to_add;
  *to_add = new Node<T>;
  to_add->value_ = v;

  if ( itr.ptr_ == 0 ) // if the iterator is null ... insert to end ? b/c end () also returns null iteratror
  {
    tail_->next_ = to_add;
    to_add->prev_ = tail_;
    tail_ = to_add;
  } else {
    // otherwise, the itr should be pointing at a node within the list
    to_right = itr.ptr_->next_;
    to_left = itr.ptr_;

    if (to_right != 0) to_right->prev_ = to_add; // want to make sure to_right is not null 
    to_left->next_ = to_add; // to_left cannot be null b/c we already cheked in 1st if -- so no need to double check

    to_add->next_ = to_right;
    to_add->prev_ = to_left;
  }

  to_return iterator(to_add);
}

#endif
