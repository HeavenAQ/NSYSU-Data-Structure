// Author: Yi-Min Chen
// Date: 2023-10-30
// Description: This program is to implement a polynomial class with linked list

#include <iostream>
#include <ostream>

using namespace std;

/* Class: Node
 * ------------
 *  A class to represent a node of a linked list
 *
 *  data: the data of the node
 *  next: the pointer to the next node
 */
template <class T> class Node
{
  public:
    T data;
    Node<T> *next;
    Node(T data, Node<T> *next)
    {
        this->data = data;
        this->next = next;
    }
};

/* Class: LinkedList
 * -----------------
 *  A class to represent a linked list
 *
 *  head: the pointer to the head node
 *  tail: the pointer to the tail node
 *  LinkedList: the constructor of the class
 */
template <class T> class LinkedList
{
  public:
    Node<T> *head;
    Node<T> *tail;
    LinkedList() : head(NULL), tail(NULL) {}
    void prepend(T data);
    void append(T data);
    void insert(Node<T> *node, T data);
    void remove(Node<T> *node);
};

// Member functions of LinkedList
// ==============================
/* Function: prepend
 * -----------------
 *  Add a new node at the beginning of the linked list
 *
 *  data: the data of the new node
 *
 *  return: void
 */
template <class T> void LinkedList<T>::prepend(T data)
{
    Node<T> *new_node = new Node<T>(data, head);
    head = new_node;
    if (!tail)
        tail = new_node;
}

/* Function: append
 * -----------------
 *  Add a new node at the end of the linked list
 *
 *  data: the data of the new node
 *
 *  return: void
 */
template <class T> void LinkedList<T>::append(T data)
{
    Node<T> *new_node = new Node<T>(data, NULL);
    if (tail)
        tail->next = new_node;

    tail = new_node;

    if (!head)
        head = new_node;
}

/* Function: insert
 * -----------------
 *  Insert a new node after the given node
 *
 *  node: the given node
 *  data: the data of the new node
 *
 *  return: void
 */
template <class T> void LinkedList<T>::insert(Node<T> *node, T data)
{
    if (node == tail) {
        append(data);
    } else if (node == head) {
        prepend(data);
    } else {
        Node<T> *new_node = new Node<T>(data, node->next);
        node->next = new_node;
    }
}

/* Function: remove
 * -----------------
 *  Remove the given node
 *
 *  node: the node to be removed
 *
 *  return: void
 */
template <class T> void LinkedList<T>::remove(Node<T> *node)
{
    if (head == NULL)
        return;

    if (head == tail) {
        head = NULL;
        tail = NULL;
        delete node;
        return;
    }

    if (node == head) {
        head = head->next;
        delete node;
    }

    for (Node<T> *cur = head; cur; cur = cur->next) {
        if (cur->next == node) {
            if (cur->next == tail)
                tail = cur;
            cur->next = cur->next->next;
            break;
        }
    }
}

/* Class: Term
 * -----------
 *  A class to represent a term of a polynomial
 *
 *  coef: the coefficient of the term
 *  exp: the exponent of the term
 */
class Term
{
  public:
    int coef;
    int exp;
    Term(){};
    Term(int coef, int exp)
    {
        this->coef = coef;
        this->exp = exp;
    }
};

/* Class: Poly
 * -----------
 *  A class to represent a polynomial
 *
 *  terms: a linked list of terms
 */
class Poly
{
  public:
    LinkedList<Term> *terms;
    Poly() { terms = new LinkedList<Term>(); }
    ~Poly() { delete terms; }
    void merge_poly(Term &rhs);
    void sorted_by_exp(void);
    void copy(Poly &rhs);
    Poly *operator+(Poly &rhs);
    Poly *operator*(Poly &rhs);
    friend istream &operator>>(istream &CIN, Poly &rhs);
    friend ostream &operator<<(ostream &COUT, const Poly &rhs);
};

// Member functions of Poly
// ========================
/* Function: copy
 * --------------
 *  Copy the polynomial from the given polynomial
 *
 *  rhs: the polynomial to copy from
 *
 *  return: void
 */
void Poly::copy(Poly &rhs)
{
    for (Node<Term> *cur = rhs.terms->head; cur; cur = cur->next)
        this->terms->append(cur->data);
}

/* Function: merge_poly
 * --------------------
 *  Merge the given term into the polynomial by adding the coefficient of the
 *  term with the term with the same exponent in the polynomial
 *
 *  rhs: the given term
 *
 *  return: void
 */
void Poly::merge_poly(Term &rhs)
{
    if (rhs.coef == 0)
        return;

    for (Node<Term> *cur = this->terms->head; cur; cur = cur->next) {
        if (cur->data.exp == rhs.exp) {
            cur->data.coef += rhs.coef;
            if (cur->data.coef == 0)
                this->terms->remove(cur);
            return;
        }
    }

    terms->append(rhs);
}

/* Function: sorted_by_exp
 * -----------------------
 *  Sort the polynomial by the exponent of the terms in descending order
 *
 *  return: void
 */
void Poly::sorted_by_exp(void)
{
    for (Node<Term> *cur = this->terms->head; cur; cur = cur->next) {
        for (Node<Term> *next = cur->next; next; next = next->next) {
            if (cur->data.exp < next->data.exp) {
                Term tmp = cur->data;
                cur->data = next->data;
                next->data = tmp;
            }
        }
    }
}

// Overloaded operators
// ====================
// Overloaded operators of Poly
// ----------------------------
/* Function: operator<< (overloaded)
 * --------------------
 *  Print the polynomial to the ostream object with the format of
 *  "coefficient exponent"
 *
 *  COUT: the ostream object
 *  rhs: the polynomial to be printed
 *
 *  return: the ostream object
 */
ostream &operator<<(ostream &COUT, Poly &rhs)
{
    for (Node<Term> *cur = rhs.terms->head; cur; cur = cur->next)
        COUT << cur->data.coef << " " << cur->data.exp << endl;
    return COUT;
}

/* Function: operator>> (overloaded)
 * --------------------
 *  Read the polynomial from the istream object with the format of
 *  total
 *  coefficient exponent
 *  ...
 *
 *  CIN: the istream object
 *  rhs: the polynomial to be read
 *
 *  return: the istream object
 */
istream &operator>>(istream &CIN, Poly &rhs)
{
    int total;
    CIN >> total;
    for (int i = 0; i < total; i++) {
        int coef, exp;
        CIN >> coef >> exp;
        Term term(coef, exp);
        rhs.merge_poly(term);
    }
    return CIN;
}

/* Function: operator+ (overloaded)
 * --------------------
 *  Add two polynomials
 *
 *  rhs: the polynomial to be added
 *
 *  return: the result polynomial
 */
Poly *Poly::operator+(Poly &rhs)
{
    Poly *p = new Poly();
    p->copy(*this);
    for (Node<Term> *cur = rhs.terms->head; cur; cur = cur->next) {
        p->merge_poly(cur->data);
    }
    return p;
}

/* Function: operator* (overloaded)
 * --------------------
 *  Multiply two polynomials
 *
 *  rhs: the polynomial to be multiplied
 *
 *  return: the result polynomial
 */
Poly *Poly::operator*(Poly &rhs)
{
    Poly *p = new Poly();
    for (Node<Term> *this_cur = this->terms->head; this_cur;
         this_cur = this_cur->next) {
        for (Node<Term> *rhs_cur = rhs.terms->head; rhs_cur;
             rhs_cur = rhs_cur->next) {
            int coef = this_cur->data.coef * rhs_cur->data.coef;
            int exp = this_cur->data.exp + rhs_cur->data.exp;
            Term term(coef, exp);
            p->merge_poly(term);
            if (this_cur->data.coef == 0)
                p->terms->remove(this_cur);
        }
    }
    return p;
}

/* Function: main
 * --------------
 *  The main function of the program
 *
 *  return: 0
 */
int main(void)
{
    int case_num = 1;
    while (true) {
        Poly p1 = Poly();
        Poly p2 = Poly();
        cin >> p1 >> p2;

        if (!p1.terms->head && !p2.terms->head)
            break;

        cout << "Case " << case_num++ << endl;
        cout << "ADD" << endl;
        Poly add_p = *(p1 + p2);
        add_p.sorted_by_exp();
        (add_p.terms->head) ? cout << add_p : cout << "0 0\n";

        cout << "MULTIPLY" << endl;
        Poly mul_p = *(p1 * p2);
        mul_p.sorted_by_exp();
        (mul_p.terms->head) ? cout << mul_p : cout << "0 0\n";
    }
    return 0;
}
