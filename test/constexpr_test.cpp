// Copyright 2023 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt)

#include <boost/crc.hpp>
#include <boost/core/lightweight_test.hpp>

#if !BOOST_CRC_CONSTEXPR_ENABLED

#include <boost/config/pragma_message.hpp>

BOOST_PRAGMA_MESSAGE( "Test skipped because constexpr usage in boost.crc is not enabled" )

#else

// The type of CRC parameters that can go in a template should be related
// on the CRC's bit count.  This macro expresses that type in a compact
// form.
#define BOOST_CRC_PARM_TYPE  typename ::boost::crc_detail::uint_t<Bits>::fast

template <std::size_t Bits,              //
          BOOST_CRC_PARM_TYPE TruncPoly, //
          BOOST_CRC_PARM_TYPE InitRem,   //
          BOOST_CRC_PARM_TYPE FinalXor,  //
          bool ReflectIn,                //
          bool ReflectRem,               //
          BOOST_CRC_PARM_TYPE CheckValue>
struct crc_traits
{
    using value_type = BOOST_CRC_PARM_TYPE;

    static constexpr size_t bit_count                           = Bits;
    static constexpr BOOST_CRC_PARM_TYPE truncated_polynominal  = TruncPoly;
    static constexpr BOOST_CRC_PARM_TYPE initial_remainder      = InitRem;
    static constexpr BOOST_CRC_PARM_TYPE final_xor_value        = FinalXor;
    static constexpr bool reflect_input                         = ReflectIn;
    static constexpr bool reflect_remainder                     = ReflectRem;
    static constexpr BOOST_CRC_PARM_TYPE check_value            = CheckValue;
};

template<typename Traits>
using crc_basic = boost::crc_basic<Traits::bit_count>;

template<typename Traits>
using crc_optimal = boost::crc_optimal<Traits::bit_count,             //
                                       Traits::truncated_polynominal, //
                                       Traits::initial_remainder,     //
                                       Traits::final_xor_value,       //
                                       Traits::reflect_input,         //
                                       Traits::reflect_remainder      //
                                       >;

template<typename Traits>
constexpr crc_basic<Traits> create_crc_basic( Traits )
{
    return crc_basic<Traits>(
        Traits::truncated_polynominal, //
        Traits::initial_remainder,     //
        Traits::final_xor_value,       //
        Traits::reflect_input,         //
        Traits::reflect_remainder      //
    );
}

template<typename Traits>
constexpr crc_optimal<Traits> create_crc_optimal( Traits, typename Traits::value_type init_rem = Traits::initial_remainder )
{
    return crc_optimal<Traits>{ init_rem };
}

// CRC-12/UMTS
// One of a few types that have opposite values of reflect_input and reflect_remainder
constexpr auto crc_12_umts_traits = crc_traits<
    /*bits*/12,
    /*truncated_polynomial*/0x80F,
    /*initial_remainder*/0x000,
    /*final_xor_value*/0x000,
    /*reflect_input*/false,
    /*reflect_remainder*/true,
    /*check_value*/0xDAF
    >();

// CRC-16/MAXIM-DOW
// Has different values in initial_remainder and final_xor_value
constexpr auto crc_16_maxim_dow_traits = crc_traits<
    /*bits*/16,
    /*truncated_polynomial*/0x8005,
    /*initial_remainder*/0x0000,
    /*final_xor_value*/0xFFFF,
    /*reflect_input*/true,
    /*reflect_remainder*/true,
    /*check_value*/0x44C2
    >();

static_assert( crc_basic<decltype(crc_12_umts_traits)>::bit_count == 12, "" );

constexpr auto basic_crc_12_umts = create_crc_basic( crc_12_umts_traits );
static_assert( basic_crc_12_umts.get_truncated_polynominal() == 0x80F, "" );
static_assert( basic_crc_12_umts.get_initial_remainder() == 0x000, "" );
static_assert( basic_crc_12_umts.get_final_xor_value() == 0x000, "" );
static_assert( basic_crc_12_umts.get_reflect_input() == false, "" );
static_assert( basic_crc_12_umts.get_reflect_remainder() == true, "" );

static_assert( crc_optimal<decltype(crc_12_umts_traits)>::bit_count == 12, "" );
static_assert( crc_optimal<decltype(crc_12_umts_traits)>::truncated_polynominal == 0x80F, "" );
static_assert( crc_optimal<decltype(crc_12_umts_traits)>::initial_remainder == 0x000, "" );
static_assert( crc_optimal<decltype(crc_12_umts_traits)>::final_xor_value == 0x000, "" );
static_assert( crc_optimal<decltype(crc_12_umts_traits)>::reflect_input == false, "" );
static_assert( crc_optimal<decltype(crc_12_umts_traits)>::reflect_remainder == true, "" );

constexpr auto optimal_crc_12_umts = create_crc_optimal( crc_12_umts_traits );
//static_assert( optimal_crc_12_umts.get_truncated_polynomial() == 0x80F, "" );
//static_assert( optimal_crc_12_umts.get_initial_remainder() == 0x000, "" );
//static_assert( optimal_crc_12_umts.get_final_xor_value() == 0x000, "" );
//static_assert( optimal_crc_12_umts.get_reflect_input() == false, "" );
//static_assert( optimal_crc_12_umts.get_reflect_remainder() == true, "" );

#endif

int main()
{
}
