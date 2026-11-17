#include "radiant/css/types/Function.h"

#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/ast/abstract_syntax_tree.h"
#include "radiant/css/types/Integer.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>

using namespace Radiant::StyleSheetParser;

Types::Integer add(Types::Integer a, Types::Integer b) {
	return Types::Integer{a + b};
}

Types::Integer sub(Types::Integer a, Types::Integer b) {
	return Types::Integer{a - b};
}

TEST_CASE("Test Function type.", "[CSS_Parser]") {
	Types::RuntimeFunction add_wrapper{registerFunction(&add)};
	Types::RuntimeFunction sub_wrapper{registerFunction(&sub)};

	Types::Function testFunction{
	    "test", add_wrapper, {StyleSheetValue{Types::Integer{10}}, StyleSheetValue{Types::Integer{10}}}};

	SECTION("Test function resolution.") {
		Radiant::StyleSheetParser::StyleSheetValue value = testFunction.resolve(0.0f);
		REQUIRE(value.isType<Types::Integer>());
		REQUIRE(value.get<Types::Integer>() == 20); // 10 + 10 == 20
	}

	SECTION("Test compound function resolution.") {
		// clang-format off
	  Types::Function testFunction{"test", add_wrapper, {
      StyleSheetValue{Types::Integer{10}}, 
      StyleSheetValue{Types::Function{"test2", sub_wrapper, {
        StyleSheetValue{Types::Integer{10}}, 
        StyleSheetValue{Types::Integer{5}} 
      }}}
    }};
		// clang-format on

		Radiant::StyleSheetParser::StyleSheetValue value = testFunction.resolve(0.0f);
		REQUIRE(value.isType<Types::Integer>());
		REQUIRE(value.get<Types::Integer>() == 15); // 10 + (10 - 5) == 15
	}
}
