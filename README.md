# Jaguar SQL Type-Checker

The aim of the project is to create a type-checker
for SQL language.

General idea:
- Migration files are seen as actions on the SQL Universe and are used to create and infer new types
- SQL strings are directly parsed and type-checked at compile-time
- SQL strings are transpiled (using annotation processing in Java) and exported in some way, using a template.

