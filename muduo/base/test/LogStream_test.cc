#include "muduo/base/LogStream.h"

#include <limits>
#include <stdint.h>

//#define BOOST_TEST_MODULE LogStreamTest
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

/*
 Format a number with 5 characters, including SI units.
 [0,     999]
 [1.00k, 999k]
 [1.00M, 999M]
 [1.00G, 999G]
 [1.00T, 999T]
 [1.00P, 999P]
 [1.00E, inf)
*/
std::string formatSI(size_t s)
{
  char buf[64];
  if (s < 1000)
    snprintf(buf, sizeof(buf), "%zd", s);
  else if (s < 9995)
    snprintf(buf, sizeof(buf), "%.2fk", s/1e3);
  else if (s < 99950)
    snprintf(buf, sizeof(buf), "%.1fk", s/1e3);
  else if (s < 999500)
    snprintf(buf, sizeof(buf), "%.0fk", s/1e3);
  else if (s < 9995000)
    snprintf(buf, sizeof(buf), "%.2fM", s/1e6);
  else if (s < 99950000)
    snprintf(buf, sizeof(buf), "%.1fM", s/1e6);
  else if (s < 999500000)
    snprintf(buf, sizeof(buf), "%.0fM", s/1e6);
  else if (s < 9995000000)
    snprintf(buf, sizeof(buf), "%.2fG", s/1e9);
  else if (s < 99950000000)
    snprintf(buf, sizeof(buf), "%.1fG", s/1e9);
  else if (s < 999500000000)
    snprintf(buf, sizeof(buf), "%.0fG", s/1e9);
  else if (s < 9995000000000)
    snprintf(buf, sizeof(buf), "%.2fT", s/1e12);
  else if (s < 99950000000000)
    snprintf(buf, sizeof(buf), "%.1fT", s/1e12);
  else if (s < 999500000000000)
    snprintf(buf, sizeof(buf), "%.0fT", s/1e12);
  else if (s < 9995000000000000)
    snprintf(buf, sizeof(buf), "%.2fP", s/1e15);
  else if (s < 99950000000000000)
    snprintf(buf, sizeof(buf), "%.1fP", s/1e15);
  else if (s < 999500000000000000)
    snprintf(buf, sizeof(buf), "%.0fP", s/1e15);
  else if (s < 9995000000000000000ULL)
    snprintf(buf, sizeof(buf), "%.2fE", s/1e18);
  else
    snprintf(buf, sizeof(buf), "%.1fE", s/1e18);
  return buf;
}

/*
std::string formatSI(size_t s)
{
  double n = s;
  char buf[64] = "";
  if (s < 100000)           // [0, 99999]
    snprintf(buf, sizeof(buf), "%zd", s);
  else if (s < 9999500)     // [ 100k, 9999k]
    snprintf(buf, sizeof(buf), "%zdk", (s+500)/1000);
  else if (s < 99950000)    // [10.0M, 99.9M]
    snprintf(buf, sizeof(buf), "%.1fM", s/1e6);
  else if (s < 9999500000)  // [ 100M, 9999M]
    snprintf(buf, sizeof(buf), "%zdM", (s+500000)/1000000);
  else if (s < 99950000000) // [10.0G, 99.9G]
    snprintf(buf, sizeof(buf), "%.1fG", n/1e9);
  else if (n < 1e13)        // [ 100G, 9999G]
    snprintf(buf, sizeof(buf), "%.0fG", n/1e9);
  else if (n < 1e14)
    snprintf(buf, sizeof(buf), "%.1fT", n/1e12);
  else if (n < 1e16)
    snprintf(buf, sizeof(buf), "%.0fT", n/1e12);
  else if (n < 1e17)
    snprintf(buf, sizeof(buf), "%.1fP", n/1e15);
  else if (n < 1e19)
    snprintf(buf, sizeof(buf), "%.0fP", n/1e15);
  else
    snprintf(buf, sizeof(buf), "%.1fE", n/1e18);
  return buf;
}
*/

/*
 [0, 1023]
 [1.00Ki, 9.99Ki]
 [10.0Ki, 99.9Ki]
 [ 100Ki, 1023Ki]
 [1.00Mi, 9.99Mi]
*/
std::string formatIEC(size_t s)
{
  double n = s;
  char buf[64];
  const double Ki = 1024.0;
  const double Mi = Ki * 1024.0;
  const double Gi = Mi * 1024.0;
  const double Ti = Gi * 1024.0;
  const double Pi = Ti * 1024.0;
  const double Ei = Pi * 1024.0;

  if (n < Ki)
    snprintf(buf, sizeof buf, "%zd", s);
  else if (s < Ki*9.995)
    snprintf(buf, sizeof buf, "%.2fKi", s / Ki);
  else if (s < Ki*99.95)
    snprintf(buf, sizeof buf, "%.1fKi", s / Ki);
  else if (s < Ki*1023.5)
    snprintf(buf, sizeof buf, "%.0fKi", s / Ki);

  else if (s < Mi*9.995)
    snprintf(buf, sizeof buf, "%.2fMi", s / Mi);
  else if (s < Mi*99.95)
    snprintf(buf, sizeof buf, "%.1fMi", s / Mi);
  else if (s < Mi*1023.5)
    snprintf(buf, sizeof buf, "%.0fMi", s / Mi);

  else if (s < Gi*9.995)
    snprintf(buf, sizeof buf, "%.2fGi", s / Gi);
  else if (s < Gi*99.95)
    snprintf(buf, sizeof buf, "%.1fGi", s / Gi);
  else if (s < Gi*1023.5)
    snprintf(buf, sizeof buf, "%.0fGi", s / Gi);

  else if (s < Ti*9.995)
    snprintf(buf, sizeof buf, "%.2fTi", s / Ti);
  else if (s < Ti*99.95)
    snprintf(buf, sizeof buf, "%.1fTi", s / Ti);
  else if (s < Ti*1023.5)
    snprintf(buf, sizeof buf, "%.0fTi", s / Ti);

  else if (s < Pi*9.995)
    snprintf(buf, sizeof buf, "%.2fPi", s / Pi);
  else if (s < Pi*99.95)
    snprintf(buf, sizeof buf, "%.1fPi", s / Pi);
  else if (s < Pi*1023.5)
    snprintf(buf, sizeof buf, "%.0fPi", s / Pi);

  else if (s < Ei*9.995)
    snprintf(buf, sizeof buf, "%.2fEi", s / Ei );
  else
    snprintf(buf, sizeof buf, "%.1fEi", s / Ei );
  return buf;
}

BOOST_AUTO_TEST_CASE(testLogStreamBooleans)
{
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();
  BOOST_CHECK_EQUAL(buf.toString(), string(""));
  os << true;
  BOOST_CHECK_EQUAL(buf.toString(), string("1"));
  os << '\n';
  BOOST_CHECK_EQUAL(buf.toString(), string("1\n"));
  os << false;
  BOOST_CHECK_EQUAL(buf.toString(), string("1\n0"));
}

BOOST_AUTO_TEST_CASE(testLogStreamIntegers)
{
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();
  BOOST_CHECK_EQUAL(buf.toString(), string(""));
  os << 1;
  BOOST_CHECK_EQUAL(buf.toString(), string("1"));
  os << 0;
  BOOST_CHECK_EQUAL(buf.toString(), string("10"));
  os << -1;
  BOOST_CHECK_EQUAL(buf.toString(), string("10-1"));
  os.resetBuffer();

  os << 0 << " " << 123 << 'x' << 0x64;
  BOOST_CHECK_EQUAL(buf.toString(), string("0 123x100"));
}

BOOST_AUTO_TEST_CASE(testLogStreamIntegerLimits)
{
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();
  os << -2147483647;
  BOOST_CHECK_EQUAL(buf.toString(), string("-2147483647"));
  os << static_cast<int>(-2147483647 - 1);
  BOOST_CHECK_EQUAL(buf.toString(), string("-2147483647-2147483648"));
  os << ' ';
  os << 2147483647;
  BOOST_CHECK_EQUAL(buf.toString(), string("-2147483647-2147483648 2147483647"));
  os.resetBuffer();

  os << std::numeric_limits<int16_t>::min();
  BOOST_CHECK_EQUAL(buf.toString(), string("-32768"));
  os.resetBuffer();

  os << std::numeric_limits<int16_t>::max();
  BOOST_CHECK_EQUAL(buf.toString(), string("32767"));
  os.resetBuffer();

  os << std::numeric_limits<uint16_t>::min();
  BOOST_CHECK_EQUAL(buf.toString(), string("0"));
  os.resetBuffer();

  os << std::numeric_limits<uint16_t>::max();
  BOOST_CHECK_EQUAL(buf.toString(), string("65535"));
  os.resetBuffer();

  os << std::numeric_limits<int32_t>::min();
  BOOST_CHECK_EQUAL(buf.toString(), string("-2147483648"));
  os.resetBuffer();

  os << std::numeric_limits<int32_t>::max();
  BOOST_CHECK_EQUAL(buf.toString(), string("2147483647"));
  os.resetBuffer();

  os << std::numeric_limits<uint32_t>::min();
  BOOST_CHECK_EQUAL(buf.toString(), string("0"));
  os.resetBuffer();

  os << std::numeric_limits<uint32_t>::max();
  BOOST_CHECK_EQUAL(buf.toString(), string("4294967295"));
  os.resetBuffer();

  os << std::numeric_limits<int64_t>::min();
  BOOST_CHECK_EQUAL(buf.toString(), string("-9223372036854775808"));
  os.resetBuffer();

  os << std::numeric_limits<int64_t>::max();
  BOOST_CHECK_EQUAL(buf.toString(), string("9223372036854775807"));
  os.resetBuffer();

  os << std::numeric_limits<uint64_t>::min();
  BOOST_CHECK_EQUAL(buf.toString(), string("0"));
  os.resetBuffer();

  os << std::numeric_limits<uint64_t>::max();
  BOOST_CHECK_EQUAL(buf.toString(), string("18446744073709551615"));
  os.resetBuffer();

  int16_t a = 0;
  int32_t b = 0;
  int64_t c = 0;
  os << a;
  os << b;
  os << c;
  BOOST_CHECK_EQUAL(buf.toString(), string("000"));
}

BOOST_AUTO_TEST_CASE(testLogStreamFloats)
{
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  os << 0.0;
  BOOST_CHECK_EQUAL(buf.toString(), string("0"));
  os.resetBuffer();

  os << 1.0;
  BOOST_CHECK_EQUAL(buf.toString(), string("1"));
  os.resetBuffer();

  os << 0.1;
  BOOST_CHECK_EQUAL(buf.toString(), string("0.1"));
  os.resetBuffer();

  os << 0.05;
  BOOST_CHECK_EQUAL(buf.toString(), string("0.05"));
  os.resetBuffer();

  os << 0.15;
  BOOST_CHECK_EQUAL(buf.toString(), string("0.15"));
  os.resetBuffer();

  double a = 0.1;
  os << a;
  BOOST_CHECK_EQUAL(buf.toString(), string("0.1"));
  os.resetBuffer();

  double b = 0.05;
  os << b;
  BOOST_CHECK_EQUAL(buf.toString(), string("0.05"));
  os.resetBuffer();

  double c = 0.15;
  os << c;
  BOOST_CHECK_EQUAL(buf.toString(), string("0.15"));
  os.resetBuffer();

  os << a+b;
  BOOST_CHECK_EQUAL(buf.toString(), string("0.15"));
  os.resetBuffer();

  BOOST_CHECK(a+b != c);

  os << 1.23456789;
  BOOST_CHECK_EQUAL(buf.toString(), string("1.23456789"));
  os.resetBuffer();

  os << 1.234567;
  BOOST_CHECK_EQUAL(buf.toString(), string("1.234567"));
  os.resetBuffer();

  os << -123.456;
  BOOST_CHECK_EQUAL(buf.toString(), string("-123.456"));
  os.resetBuffer();
}

BOOST_AUTO_TEST_CASE(testLogStreamVoid)
{
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  os << static_cast<void*>(0);
  BOOST_CHECK_EQUAL(buf.toString(), string("0x0"));
  os.resetBuffer();

  os << reinterpret_cast<void*>(8888);
  BOOST_CHECK_EQUAL(buf.toString(), string("0x22B8"));
  os.resetBuffer();
}

BOOST_AUTO_TEST_CASE(testLogStreamStrings)
{
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  os << "Hello ";
  BOOST_CHECK_EQUAL(buf.toString(), string("Hello "));

  string chenshuo = "Shuo Chen";
  os << chenshuo;
  BOOST_CHECK_EQUAL(buf.toString(), string("Hello Shuo Chen"));
}

/*
BOOST_AUTO_TEST_CASE(testLogStreamFmts)
{
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  os << Fmt("%4d", 1);
  BOOST_CHECK_EQUAL(buf.toString(), string("   1"));
  os.resetBuffer();

  os << Fmt("%4.2f", 1.2);
  BOOST_CHECK_EQUAL(buf.toString(), string("1.20"));
  os.resetBuffer();

  os << Fmt("%4.2f", 1.2) << Fmt("%4d", 43);
  BOOST_CHECK_EQUAL(buf.toString(), string("1.20  43"));
  os.resetBuffer();
}
*/
BOOST_AUTO_TEST_CASE(testLogStreamLong)
{
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();
  for (int i = 0; i < 399; ++i)
  {
    os << "123456789 ";
    BOOST_CHECK_EQUAL(buf.length(), 10*(i+1));
    BOOST_CHECK_EQUAL(buf.avail(), 4000 - 10*(i+1));
  }

  os << "abcdefghi ";
  BOOST_CHECK_EQUAL(buf.length(), 3990);
  BOOST_CHECK_EQUAL(buf.avail(), 10);

  os << "abcdefghi";
  BOOST_CHECK_EQUAL(buf.length(), 3999);
  BOOST_CHECK_EQUAL(buf.avail(), 1);
}

BOOST_AUTO_TEST_CASE(testFormatSI)
{
  BOOST_CHECK_EQUAL(formatSI(0), string("0"));
  BOOST_CHECK_EQUAL(formatSI(999), string("999"));
  BOOST_CHECK_EQUAL(formatSI(1000), string("1.00k"));
  BOOST_CHECK_EQUAL(formatSI(9990), string("9.99k"));
  BOOST_CHECK_EQUAL(formatSI(9994), string("9.99k"));
  BOOST_CHECK_EQUAL(formatSI(9995), string("10.0k"));
  BOOST_CHECK_EQUAL(formatSI(10000), string("10.0k"));
  BOOST_CHECK_EQUAL(formatSI(10049), string("10.0k"));
  BOOST_CHECK_EQUAL(formatSI(10050), string("10.1k"));
  BOOST_CHECK_EQUAL(formatSI(99900), string("99.9k"));
  BOOST_CHECK_EQUAL(formatSI(99949), string("99.9k"));
  BOOST_CHECK_EQUAL(formatSI(99950), string("100k"));
  BOOST_CHECK_EQUAL(formatSI(100499), string("100k"));
  // FIXME:
  // BOOST_CHECK_EQUAL(formatSI(100500), string("101k"));
  BOOST_CHECK_EQUAL(formatSI(100501), string("101k"));
  BOOST_CHECK_EQUAL(formatSI(999499), string("999k"));
  BOOST_CHECK_EQUAL(formatSI(999500), string("1.00M"));
  BOOST_CHECK_EQUAL(formatSI(1004999), string("1.00M"));
  // BOOST_CHECK_EQUAL(formatSI(1005000), string("1.01M"));
  BOOST_CHECK_EQUAL(formatSI(1005001), string("1.01M"));
  BOOST_CHECK_EQUAL(formatSI(INT64_MAX), string("9.22E"));
}

BOOST_AUTO_TEST_CASE(testFormatIEC)
{
  BOOST_CHECK_EQUAL(formatIEC(0), string("0"));
  BOOST_CHECK_EQUAL(formatIEC(1023), string("1023"));
  BOOST_CHECK_EQUAL(formatIEC(1024), string("1.00Ki"));
  BOOST_CHECK_EQUAL(formatIEC(10234), string("9.99Ki"));
  BOOST_CHECK_EQUAL(formatIEC(10235), string("10.0Ki"));
  BOOST_CHECK_EQUAL(formatIEC(10240), string("10.0Ki"));
  BOOST_CHECK_EQUAL(formatIEC(10291), string("10.0Ki"));
  BOOST_CHECK_EQUAL(formatIEC(10292), string("10.1Ki"));
  BOOST_CHECK_EQUAL(formatIEC(102348), string("99.9Ki"));
  BOOST_CHECK_EQUAL(formatIEC(102349), string("100Ki"));
  BOOST_CHECK_EQUAL(formatIEC(102912), string("100Ki"));
  BOOST_CHECK_EQUAL(formatIEC(102913), string("101Ki"));
  BOOST_CHECK_EQUAL(formatIEC(1022976), string("999Ki"));
  BOOST_CHECK_EQUAL(formatIEC(1047552), string("1023Ki"));
  BOOST_CHECK_EQUAL(formatIEC(1047961), string("1023Ki"));
  BOOST_CHECK_EQUAL(formatIEC(1048063), string("1023Ki"));
  BOOST_CHECK_EQUAL(formatIEC(1048064), string("1.00Mi"));
  BOOST_CHECK_EQUAL(formatIEC(1048576), string("1.00Mi"));
  BOOST_CHECK_EQUAL(formatIEC(10480517), string("9.99Mi"));
  BOOST_CHECK_EQUAL(formatIEC(10480518), string("10.0Mi"));
  BOOST_CHECK_EQUAL(formatIEC(INT64_MAX), string("8.00Ei"));
}
