
Ponder Changelog
----------------

- [Notes with examples on blog](http://billyquith.github.io/ponder/blog/).

## 3.0

- Extensive refactoring of the API.
	- Changed UserObject reference semantics. Pointer makes a reference.
- All types have common ancestor: Type.
- Added UserData per Type. (#98)
- Refactored serialisation. Removed ponder-xml. (#43).
- Added RapidJSON for use by JSON serialiser (#43).
- Added RapidXML for use by XML serialiser (#43).
- Remove dynamic read/write attribute from properties. (#91)
- Remove TagHolder. (#92)
- Removed `ponder::classByIndex()`. Use `ponder::classIterator()` instead.
- Cmake project name changed to "ponder" to match namespace (#95).
- Removed composed property getter. Use lambdas instead. See property test p14-p17.

### 2.2

- Removed dependency on cppformat (#59)

#### 2.1.1

- Identifiers/names now return IdReturn (`const std::string&`) to improve usability.

### 2.1

- Bind Lua argument convertion directly to function. Saves Value conversion.
- Lua custom type conversion with LuaValueRead and LuaValueWrite.
- Lua supports enums.
- Lua can return multiple values via `std::tuple<...>`.
- Lua functions can parse table arguments.

## 2.0

- Version 1.4 was unreleased and morphed into 2.0 because of extensive design changes.
- Runtime functionality (object factory and function calling) decoupled (#49).
- Many things renamed in the hope that this will clarify things.
  - FunctionType now FunctionKind. Avoid clash with "type".
  - ValueType now ValueKind. Avoid clash with "type".
  - Arguments now parameters. Functions have parameters and are called with arguments.
- Added support for binding to static functions (#46).
- Lua binding generator. Functional but sparse functionality. (#40).
- `PONDER_RTTI` renamed to `PONDER_POLYMORPHIC` (#36).
- Added function call return policies (#51).
- Removed UserObject parent-child. Simplification, unused.
- Removed Function callable behaviour. Unused complication.
- Class::create() allows object creation without an Args list.
- Class::tryFunction() and Class::tryProperty() added.
- Remove properties with 3 accessors. Simplification, use lambdas.


### 1.3

- Ids/Names now configurable using `IdTrait`s.
- Default `IdTrait` uses `string_view` so is more efficient (#11).
- Lambda functions recognised as class functions.
- `std::auto_ptr` removed as deprecated in C++11 and causes warning spam in GCC.
- Internal string_view class added as std one still experimental.
- Function and object classifiers.
- Bug fixes.

### 1.2

- Properties now support method chaining or named parameter idiom (#28).
- UserObjects can be created from user pointers (#30).
- Change `ponder::Type` to enum class `ValueType` (#26).
- Remove composed function bindings. Use lambdas instead.
- Re-add function unit tests.

### 1.1

- Placement new creation of objects added.
- Class function and property iterators added.
- Class function and property access by index added.
- Ability to undeclare Classes and Enums.
- Dictionaries ordered internally. Allows for fast binary searching of keys.
- Documentation fixes.

#### 1.0.2

- Moved to Catch for unit testing.

## 1.0

- Boost dependency removed.
