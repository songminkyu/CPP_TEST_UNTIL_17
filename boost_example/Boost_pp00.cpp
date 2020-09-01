#include <boost/preprocessor/cat.hpp>                           ///< PP_CAT
#include <boost/preprocessor/repetition/repeat.hpp>             ///< PP_REPEAT
#include <boost/preprocessor/repetition/enum.hpp>               ///< PP_ENUM
#include <boost/preprocessor/repetition/enum_params.hpp>        ///< PP_ENUM_PARAMS
#include <boost/preprocessor/repetition/enum_binary_params.hpp> ///< PP_ENUM_BINARY_PARAMS
#include <boost/preprocessor/punctuation/comma_if.hpp>          ///< PP_COMMA_IF
#include <iostream>
#include <type_traits>
#define OBJECT_MEMBERS_COUNT 3

#define ObjectMembers_DECL(z, n, _) BOOST_PP_CAT(ARG, n) BOOST_PP_CAT(m_Arg, n);
#define ObjectMembers_INIT(z, n, _) BOOST_PP_CAT(m_Arg, n)(BOOST_PP_CAT(arg, n))

#define DECLARE_OBJECT_MEMBER_CLASS(z, n, _)    \
template<typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename ARG)>     \
class ObjectMembers##n      \
{   \
  private:          \
    T& m_Obj;		\
    BOOST_PP_REPEAT(n, ObjectMembers_DECL, _)   \
  public: \
    ObjectMembers##n(T& obj BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_BINARY_PARAMS(n, ARG, arg)) \
    :   \
    m_Obj(&ARG) BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, ObjectMembers_INIT, _) {	\
	}  \
};

BOOST_PP_REPEAT(OBJECT_MEMBERS_COUNT, DECLARE_OBJECT_MEMBER_CLASS, _)

int main() {
	

	int obj = 1;
	ObjectMembers0<int>           om0(obj);
	ObjectMembers1<int, int>      om1(obj, 3);
	ObjectMembers2<int, int, int> om2(obj, 3, 5);

}