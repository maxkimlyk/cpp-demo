// --- Concepts ---
//
// Value access concepts:
//
//   Suppose X is an iterator type,  R is std::iterator_traits<X>::reference,
//   T is std::iterator_traits<X>::value_type, v is a constant object of type T,
//   M is type of member m of class T.
//
//   Readable Iterator:
//   1) X is assignable and copy constructible
//   2) iterator_traits<X>::value_type (or T) is any non-reference, non-cv-qualified type
//   3) *a is convertible to T. If a == b then *a is equivalent to *b
//   4) a->m returns M&, and is equivalent to (*a).m
//
//   Writable Iterator:
//   1) X is copy constructible
//   2) defined *a = o for each type o of associated set of value types
//
//   Swapable Iterator:
//   1) X is copy constructible
//   2) iter_swap(a, b) -> void  is defined
//
//   Lvalue Iterator:
//   1) *a -> T&
//   2) If X is Writable Iterator then  a == b <=> *a and *b is the same object.
//      If X is Readable Iterator then  a == b => *a and *b is the same object.
//
//
// Iterator traversal concepts:
//
//   Suppose X is an iterator type, a and b are constant objects of type X,
//   r and s are mutable objects of type X, T is std::iterator_traits<X>::value_type,
//   and v is a constant object of type T.
//
//   Incrementable Iterator:
//   1) X is assignable and copy constructible
//   2) ++r -> X&
//   3) &r == &++r
//   4) r++ is valid expression
//   5) *r++ is valid expression
//   6) iterator_traversal<X>::type is convertible to incrementable_traversal_tag
//
//   Single Pass Iterator:
//   1) X is Incrementable Iterator
//   2) X is equality comparable
//   3) ++r -> X&, besides if r is dereferenceable before, then r is dereferenceable or pass-to-end after
//   4) a == b  is convertible to bool
//   5) a != b  is convertible to bool
//   6) defined iterator_traits<X>::difference_type
//   7) iterator_traversal<X>::type is convertible to single_pass_iterator_tag
//
//   Forward Traversal Iterator:
//   1) X is Single pass Iterator
//   2) X is default constructible
//   3) X u; -> X&  (u may have singular value)
//   4) ++r -> X&
//   5) r == s and r is dereferenceable => ++r == ++s
//   6) iterator_traversal<X>::type is convertible to forward_traversal_tag
//
//   Bidirectional Traversal Iterator:
//   1) X is Forward Traversal Iterator
//   2) --r -> X&;
//        there exists s such that r == ++s  =>  s is dereferenceable
//        ++(--r) == r
//        --r == --s  =>  r == s
//        &r == &--r;
//   3) r-- returns convertible to const X&
//        and represents the following semantics:
//        {
//          X tmp = r;
//          --r;
//          return tmp;
//        }
//   4) iterator_traversal<X>::type is convertible to bidirectional_traversal_tag
//
//   Random Access Traversal Iterator:
//   Suppose Distance is iterator_traits<X>::dirrefence_type, n is an object of type const Distance.
//   1) X is Bidirectional Traversal Iterator
//   2) r += n -> X&
//        and represents the following semantics:
//        {
//          Distance m = n;
//          if (m > 0) {
//            while (m--)
//              ++r;
//          } else {
//            while (m++) {
//              --r;
//            }
//          }
//          return r;
//        }
//   3) a + n, n + a -> X
//        and represents the following semantics:
//        {
//          X tmp = a;
//          return tmp += n;
//        }
//   4) r -= n -> X&
//        and represents the following semantics:
//        { return r += -n; }
//   5) a - n -> X
//        and represents the following semantics:
//        {
//          X tmp = a;
//          return tmp -= n;
//        }
//   6) a < b -> bool , a > b -> bool
//   7) a >= b -> bool , a <= b -> bool
//   8) b - a -> Distance
//        and represents the following semantics:
//        { return a < b ? distance(a, b) : -distance(b,a); }
//   9) if a is Readable Iterator,  a[n] -> convertible to T
//      if a is Writable Iterator,  a[n] = v -> convertible to T
//   10) iterator_traversal<X>::type is convertible to random_access_traversal_tag
//

