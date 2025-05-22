#include "../include/Int.h"
#include "../include/Point.h"
#include "../include/secp256k1.h"
#include <iostream>
#include <string>
#include <cstring>
#include <map>

// ANSI color codes
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static int failures = 0;
static int test_count = 0;
static std::string current_category = "";
static std::map<std::string, int> category_counts;
static std::map<std::string, int> category_failures;

#define ASSERT_EQ_STR(a, b, name) do { \
    test_count++; \
    category_counts[current_category]++; \
    std::string _aa=(a); std::string _bb=(b); \
    if(_aa!=_bb){ \
        std::cout << ANSI_COLOR_RED << "[FAIL] " << ANSI_COLOR_RESET << name << " (" << __LINE__ << "): " << _aa << " != " << _bb << std::endl; \
        failures++; \
        category_failures[current_category]++; \
    } else { \
        std::cout << ANSI_COLOR_GREEN << "[PASS] " << ANSI_COLOR_RESET << name << std::endl; \
    } \
} while(0)

#define ASSERT_TRUE(cond, name) do { \
    test_count++; \
    category_counts[current_category]++; \
    if(!(cond)){ \
        std::cout << ANSI_COLOR_RED << "[FAIL] " << ANSI_COLOR_RESET << name << " (" << __LINE__ << "): " << #cond << std::endl; \
        failures++; \
        category_failures[current_category]++; \
    } else { \
        std::cout << ANSI_COLOR_GREEN << "[PASS] " << ANSI_COLOR_RESET << name << std::endl; \
    } \
} while(0)

#define TEST_CATEGORY(name) \
    if (current_category != "") { \
        int cat_pass = category_counts[current_category] - category_failures[current_category]; \
        std::cout << ANSI_COLOR_CYAN << "   " << cat_pass << "/" << category_counts[current_category] \
        << " tests passed in this category" << ANSI_COLOR_RESET << std::endl; \
    } \
    current_category = name; \
    category_counts[current_category] = 0; \
    category_failures[current_category] = 0; \
    std::cout << ANSI_COLOR_BLUE << "\n=== " << name << " ===" << ANSI_COLOR_RESET << std::endl;

int main() {
    std::cout << ANSI_COLOR_YELLOW << "=== Running Tests ===" << ANSI_COLOR_RESET << std::endl;

    TEST_CATEGORY("Int Arithmetic Operations");
    // Int addition on large numbers
    Int a; a.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE");
    Int b(a); b.AddOne();
    ASSERT_EQ_STR(b.GetBase16(), "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "AddOne to max-1");

    Int c; c.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    Int d(c); d.AddOne();
    ASSERT_EQ_STR(d.GetBase16(), "10000000000000000000000000000000000000000000000000000000000000000", "AddOne with carry");

    Int e; e.SetBase16((char*)"8000000000000000000000000000000000000000000000000000000000000000");
    e.SubOne();
    ASSERT_EQ_STR(e.GetBase16(), "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "SubOne from large number");

    TEST_CATEGORY("Int Bit Operations");
    Int s((uint64_t)1);
    s.ShiftL(255);
    ASSERT_EQ_STR(s.GetBase16(), "8000000000000000000000000000000000000000000000000000000000000000", "ShiftLeft 255 bits");
    s.ShiftR(255);
    ASSERT_EQ_STR(s.GetBase16(), "1", "ShiftRight 255 bits");

    TEST_CATEGORY("Int Multiplication");
    Int m; m.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    Int r; r.Mult(&m,&m);
    ASSERT_EQ_STR(r.GetBase16(), "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE00000000000000000000000000000001", "Multiplication of large numbers");

    TEST_CATEGORY("Point Basic Operations");
    // Point basic functions
    Point p0; p0.Clear();
    ASSERT_TRUE(p0.isZero(), "Point Clear and isZero");

    Int px((uint64_t)1), py((uint64_t)2), pz((uint64_t)3);
    Point p1(&px,&py,&pz); Point p2(p1);
    ASSERT_TRUE(p1.equals(p2), "Point equality");

    TEST_CATEGORY("Point Reduction");
    // Point reduction with big numbers
    Int P; P.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
    Int::SetupField(&P);
    Int rx; rx.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    Int ry; ry.SetBase16((char*)"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEE");
    Int rz((uint64_t)2);
    Point pr(&rx,&ry,&rz);
    pr.Reduce();
    ASSERT_EQ_STR(pr.x.GetBase16(), "800001E8", "Point reduction x-coordinate");
    ASSERT_EQ_STR(pr.y.GetBase16(), "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7", "Point reduction y-coordinate");
    ASSERT_EQ_STR(pr.z.GetBase16(), "1", "Point reduction z-coordinate");

    TEST_CATEGORY("Secp256K1 Public Key Computation");
    // Scalar multiplication with a big number
    Secp256K1 *k = new Secp256K1();
    k->Init();
    Int privkey;
    std::string privkey_str = "E0BB18B3FB477B4C6D08594DB92382EAED6B1A7D25AD140AC54A08DBE98A8C16";
    char privkey_cstr[65];
    strcpy(privkey_cstr, privkey_str.c_str());
    privkey.SetBase16(privkey_cstr);
    ASSERT_EQ_STR(privkey.GetBase16(), privkey_str, "PrivKey SetBase16");
    Point result = k->ComputePublicKey(&privkey, true);
    ASSERT_EQ_STR(result.x.GetBase16(), "8E19DF2C61EAC72C6E827814E2909A6940477473DF44627C3D32A2EABF6015B4", "PubKey x-coordinate");
    ASSERT_EQ_STR(result.y.GetBase16(), "AC701D09F46C287D87DFEDDC1623F84BDE8722E472E916DB8DFDFD7BE1ED9C03", "PubKey y-coordinate");
    ASSERT_EQ_STR(result.z.GetBase16(), "1", "PubKey z-coordinate");

    // Print final category summary
    int cat_pass = category_counts[current_category] - category_failures[current_category];
    std::cout << ANSI_COLOR_CYAN << "   " << cat_pass << "/" << category_counts[current_category] 
              << " tests passed in this category" << ANSI_COLOR_RESET << std::endl;

    // Print summary
    std::cout << ANSI_COLOR_YELLOW << "\n=== Test Summary ===" << ANSI_COLOR_RESET << std::endl;
    if(failures==0){
        std::cout << ANSI_COLOR_GREEN << "✓ All " << test_count << " tests passed!" << ANSI_COLOR_RESET << std::endl;
    } else {
        std::cout << ANSI_COLOR_RED << "✗ " << failures << " of " << test_count << " tests failed!" << ANSI_COLOR_RESET << std::endl;
    }
    return failures == 0 ? 0 : 1;
}
