#ifndef EXPECT_HPP
# define EXPECT_HPP



#include <cxxabi.h>



#define expect(expression, value) expect_((expression), (value), __FILE__, __LINE__, #expression)
#define expect_throw(expression, exception) expect_throw_<exception>([&]() { (expression); }, __FILE__, __LINE__, #expression)


std::string
demangle(char const *id)
{
    int status(-1);
    char *namep = abi::__cxa_demangle(id, 0, 0, &status);

    std::string name(namep);
    free(namep);

    return name;
}


template<class E, class C>
bool expect_throw_(
        C callable,
        char const *file,
        size_t line,
        char const *expression)
{
    enum throw_result { THROWN_NONE = 0, THROWN_EXPECTED, THROWN_OTHER };

    throw_result result(THROWN_NONE);
    std::string actual("unknown");

    try {
        callable();
    } catch(E &e) {
        result = THROWN_EXPECTED;
    } catch(std::exception &e) {
        result = THROWN_OTHER;
        actual = demangle(typeid(e).name());
    } catch(...) {
        result = THROWN_OTHER;
    }

    if (THROWN_EXPECTED != result) {
        std::cout << file << ':' << line << ':'
            << '\t' << "expression: " << expression << "\n"
            << '\t' << "expected exception: " << demangle(typeid(E).name()) << "\n"
            << '\t' << "acutal exception: " << actual << std::endl;

        return false;
    }

    return true;
}


template<class T, class U>
bool expect_(
        T const &actual,
        U const &value,
        char const *file,
        size_t line,
        char const *expression)
{
    if (value != actual) {
        std::cout << file << ':' << line << ':'
            << '\t' << "expression: " << expression << "\n"
            << '\t' << "expected: " << value << '\n'
            << '\t' << "got: " << actual << '\n' << std::endl;

        return false;
    }

    return true;
}



#endif /* EXPECT_HPP */
