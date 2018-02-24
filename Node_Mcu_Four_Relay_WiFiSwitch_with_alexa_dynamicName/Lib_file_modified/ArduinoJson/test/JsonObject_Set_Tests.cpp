// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <gtest/gtest.h>
#define ARDUINOJSON_ENABLE_STD_STREAM
#include <ArduinoJson.h>

class JsonObject_Set_Tests : public ::testing::Test {
 public:
  JsonObject_Set_Tests() : _object(_jsonBuffer.createObject()) {}

 protected:
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object;
};

#define TEST_(name) TEST_F(JsonObject_Set_Tests, name)

TEST_(SizeIncreased_WhenValuesAreAdded) {
  _object.set("hello", 42);
  EXPECT_EQ(1, _object.size());
}

TEST_(SizeUntouched_WhenSameValueIsAdded) {
  _object["hello"] = 1;
  _object["hello"] = 2;
  EXPECT_EQ(1, _object.size());
}

TEST_(StoreInteger) {
  _object.set("hello", 123);

  EXPECT_EQ(123, _object["hello"].as<int>());
  EXPECT_TRUE(_object["hello"].is<int>());
  EXPECT_FALSE(_object["hello"].is<double>());
}

TEST_(StoreDouble) {
  _object.set("hello", 123.45);

  EXPECT_EQ(123.45, _object["hello"].as<double>());
  EXPECT_TRUE(_object["hello"].is<double>());
  EXPECT_FALSE(_object["hello"].is<long>());
}

TEST_(StoreDoubleWithDigits) {
  _object.set("hello", 123.45, 2);

  EXPECT_EQ(123.45, _object["hello"].as<double>());
  EXPECT_TRUE(_object["hello"].is<double>());
  EXPECT_FALSE(_object["hello"].is<long>());
}

TEST_(StoreBoolean) {
  _object.set("hello", true);

  EXPECT_TRUE(_object["hello"].as<bool>());
  EXPECT_TRUE(_object["hello"].is<bool>());
  EXPECT_FALSE(_object["hello"].is<long>());
}

TEST_(StoreString) {
  _object.set("hello", "h3110");

  EXPECT_STREQ("h3110", _object["hello"].as<const char*>());
  EXPECT_TRUE(_object["hello"].is<const char*>());
  EXPECT_FALSE(_object["hello"].is<long>());
}

TEST_(StoreArray) {
  JsonArray& arr = _jsonBuffer.createArray();

  _object.set("hello", arr);

  EXPECT_EQ(&arr, &_object["hello"].asArray());
  EXPECT_TRUE(_object["hello"].is<JsonArray&>());
  EXPECT_FALSE(_object["hello"].is<JsonObject&>());
}

TEST_(StoreObject) {
  JsonObject& obj = _jsonBuffer.createObject();

  _object.set("hello", obj);

  EXPECT_EQ(&obj, &_object["hello"].asObject());
  EXPECT_TRUE(_object["hello"].is<JsonObject&>());
  EXPECT_FALSE(_object["hello"].is<JsonArray&>());
}

TEST_(StoreArraySubscript) {
  JsonArray& arr = _jsonBuffer.createArray();
  arr.add(42);

  _object.set("a", arr[0]);

  EXPECT_EQ(42, _object["a"]);
}

TEST_(StoreObjectSubscript) {
  JsonObject& obj = _jsonBuffer.createObject();
  obj.set("x", 42);

  _object.set("a", obj["x"]);

  EXPECT_EQ(42, _object["a"]);
}
