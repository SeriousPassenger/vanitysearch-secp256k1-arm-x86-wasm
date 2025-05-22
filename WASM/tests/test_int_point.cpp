#include "Int.h"
#include "Point.h"
#include <iostream>
#include <string>

static int failures = 0;
#define ASSERT_EQ_STR(a,b) do { std::string _aa=(a); std::string _bb=(b); if(_aa!=_bb){ std::cerr<<"Assertion failed ("<<__LINE__<<"): "<<_aa<<" != "<<_bb<<std::endl; failures++; }} while(0)
#define ASSERT_TRUE(cond) do { if(!(cond)){ std::cerr<<"Assertion failed ("<<__LINE__<<"): "<<#cond<<std::endl; failures++; }} while(0)

int main() {
    // Int addition on large numbers
    Int a; a.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE");
    Int b(a); b.AddOne();
    ASSERT_EQ_STR(b.GetBase16(), "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    Int c; c.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    Int d(c); d.AddOne();
    ASSERT_EQ_STR(d.GetBase16(), "10000000000000000000000000000000000000000000000000000000000000000");

    Int e; e.SetBase16((char*)"8000000000000000000000000000000000000000000000000000000000000000");
    e.SubOne();
    ASSERT_EQ_STR(e.GetBase16(), "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    Int s((uint64_t)1);
    s.ShiftL(255);
    ASSERT_EQ_STR(s.GetBase16(), "8000000000000000000000000000000000000000000000000000000000000000");
    s.ShiftR(255);
    ASSERT_EQ_STR(s.GetBase16(), "1");

    Int m; m.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    Int r; r.Mult(&m,&m);
    ASSERT_EQ_STR(r.GetBase16(), "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE00000000000000000000000000000001");

    // Point basic functions
    Point p0; p0.Clear();
    ASSERT_TRUE(p0.isZero());

    Int px((uint64_t)1), py((uint64_t)2), pz((uint64_t)3);
    Point p1(&px,&py,&pz); Point p2(p1);
    ASSERT_TRUE(p1.equals(p2));

    // Point reduction with big numbers
    Int P; P.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
    Int::SetupField(&P);
    Int rx; rx.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    Int ry; ry.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEE");
    Int rz((uint64_t)2);
    Point pr(&rx,&ry,&rz);
    pr.Reduce();
    ASSERT_EQ_STR(pr.x.GetBase16(), "800001E8");
    ASSERT_EQ_STR(pr.y.GetBase16(), "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7");
    ASSERT_EQ_STR(pr.z.GetBase16(), "1");

    if(failures==0){
        std::cout << "All tests passed" << std::endl;
    }
    return failures == 0 ? 0 : 1;
}
