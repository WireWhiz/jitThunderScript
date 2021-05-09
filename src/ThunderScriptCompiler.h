#pragma once

#ifndef TS_COMPILER
#define TS_COMPILER
#include "thunderscript.h"
#include <string>
#include <iostream>

#include "bison/bison.tab.hh"
#if !defined(yyFlexLexerOnce)
#include "bison/FlexLexer.h"
#endif

namespace ts
{

	class tsCompileError
	{
	public:
		std::string message;
		size_t line;
		tsCompileError(const std::string& _message, size_t _line)
		{
			message = _message;
			line = _line;
		}
		void display()
		{
			std::cout << "Compiler failed with message:\n" << message
				<< "\nOn line:\n" << line << std::endl;
		}
	};

	struct tsVar
	{
	public:
		std::string identifier;
		tsIndex index;
		tsVarType type;
		bool inUse = false;
		bool constant = false;
		bool initalized = false;
		bool isTemp = false;
		int size = 0;
		size_t varIndex = 0;
	};

	class tsVarPool
	{
	private:
		std::vector<tsVar> vars;
		size_t bytes = 0;
		unsigned int tempIndex = 0;
		std::stack<std::vector<tsVar*>> scopes;
	public:
		tsVarPool();
		void reset();
		size_t sizeOf();
		void enterScope();
		void exitScope();

		tsVar requestTempVar(tsVarType type, size_t line);
		tsVar requestVar( const std::string& identifier, tsVarType type, size_t line, bool isConstant = false, bool isInitalized = false);
		tsVar requestInlineConst(const std::string& identifier, tsVarType type, size_t line);

		void initialize(tsVar& var);
		void setTemp(tsVar& var, bool temp);
		bool getVarFromIdentifier(std::string identifier, tsVar& var);
		tsVar getVarFromIndex(size_t index);

	};


	class tsScanner : public yyFlexLexer
	{
	public:
		tsScanner(std::istream* in);
		using FlexLexer::yylex;
		virtual int yylex(tsParser::semantic_type* const lval, tsParser::location_type* location);

	private:
		tsParser::semantic_type* yylval = nullptr;
		/* location ptr */
		tsParser::location_type* loc = nullptr;
	};


	class tsCompiler
	{
	protected:
		std::shared_ptr<tsContext> _context;
		tsScanner* scanner = nullptr;
		tsParser* parser = nullptr;
		tsVarPool vars;
		std::unique_ptr<tsScript> script;
		
	public:
		tsCompiler(std::shared_ptr<tsContext>& context);
		~tsCompiler();

		bool compileFile(const std::string& path);

		bool compile(std::string& scriptText);

	#pragma region PreProcessing
		void removeComments(std::string& scriptText);

	#pragma endregion


	#pragma region BisonFunctions
		void generateGlobal(const std::string& identifier, const tsVarType& type, const tsGlobal::GlobalType writeMode, const std::size_t line);

		tsVar generateVar(const std::string& identifier, const tsVarType& type);

		std::size_t getVarIndex(const std::string& identifier);
		tsVar getVar(const std::size_t& index);

		void assignVar(std::size_t ai, std::size_t bi, size_t line);
		void assignVar(tsVar a, tsVar b, size_t line);
		void enterScope();
		void exitScope();

		void castVar(tsVar& var, tsVarType targetType, size_t line);

		size_t getConst(const std::string value, tsVarType type, size_t line);

		size_t add(size_t a, size_t b, size_t line);
		size_t sub(size_t a, size_t b, size_t line);

	#pragma endregion
		
		/*
	#pragma region expression operations
		// 0: variables, 1: add and subtract, 2: multiply and divide, 3: negate, 4: scope
		class tsOperation
		{
		public:
			enum class DepSide
			{
				none,
				left,
				right,
				both
			};
			size_t line;
			virtual unsigned int getPriority() = 0;
			virtual DepSide getDepSide()
			{
				return DepSide::none;
			};
			virtual tsVar getValue(tsBytecode& bytecode, tsVarPool& vars) = 0;
		};

		// An operation dependant upon one other operation
		class tsDOperator : public tsOperation
		{
		protected:
			std::unique_ptr<tsOperation> _dep;
		public:
			void setDep(std::unique_ptr<tsOperation>& dep)
			{
				_dep = std::move(dep);
			}
		};

		// An operator with a dependancy on the left
		class tsLOperator : public tsDOperator
		{
		public:
			DepSide getDepSide()
			{
				return DepSide::left;
			}
		};
		// An operator with a dependancy on the right
		class tsROperator : public tsDOperator
		{
		public:
			DepSide getDepSide()
			{
				return DepSide::right;
			}
		};

		// An operation that has two dependancies
		class tsDDOperator : public tsOperation
		{
		protected:
			std::unique_ptr<tsOperation> _dep1;
			std::unique_ptr<tsOperation> _dep2;
		public:
			DepSide getDepSide()
			{
				return DepSide::both;
			}
			tsVarType GetStdEqType(const tsVar& a, const tsVar& b)
			{
				if (a.type == b.type)
					return a.type;
				if (a.type == tsVarType::tsFloat || b.type == tsVarType::tsFloat)
					return tsVarType::tsFloat;
				else
					return tsVarType::tsNone;
			}
			void setDeps(std::unique_ptr<tsOperation>& dep1, std::unique_ptr<tsOperation>& dep2)
			{
				tsMASSERT((dep1.get() != nullptr) && (dep2.get() != nullptr), "Input pointer(s) were null");
				_dep1 = std::move(dep1);
				_dep2 = std::move(dep2);
			}
		};
		class tsScopeOperation : public tsOperation
		{
		public:
			std::unique_ptr<tsOperation> operation;
			unsigned int getPriority()
			{
				return 5;
			}
			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				return operation->getValue(bytecode, vars);
			}
		};
		class tsVarOperation : public tsOperation
		{
		public:
			tsVar _var;
			tsVarOperation(tsVar& var)
			{
				_var = var;
			}
			unsigned int getPriority()
			{
				return 1;
			}
			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				return _var;
			}
		};
		class tsAssignOperation : public tsDDOperator
		{
		public:
			unsigned int getPriority()
			{
				return 0;
			}
			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Writing assign code\n";
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();
				
				if (a.constant)
					throw tsCompileError("Can not assign a value to constant var: " + a.identifier, line);

				if (!b.initalized)
					throw tsCompileError("Use of uninitalized variable: " + b.identifier, line);

				vars.initialize(a);

				switch (a.type)
				{
					case tsVarType::tsFloat:
						b = vars.cast<tsVarType::tsFloat>(bytecode, b, line);
						bytecode.pushCmd(tsMOVE, sizeof(float), b.index, a.index);
						break;
					case tsVarType::tsInt:
						b = vars.cast<tsVarType::tsInt>(bytecode, b, line);
						bytecode.pushCmd(tsMOVE, sizeof(int), b.index, a.index);
						break;
					case tsVarType::tsBool:
						if(b.type != tsVarType::tsBool)
							throw tsCompileError("Can not auto cast type " + static_cast<std::string>(getVarTypeName(b.type)) + " to boolian", line);
						bytecode.pushCmd(tsMOVE, sizeof(bool), b.index, a.index);
						break;
					default:
						throw tsCompileError("Can not assign type " + static_cast<std::string>(getVarTypeName(b.type)) + " to " + static_cast<std::string>(getVarTypeName(a.type)), line);
				}
				a.type = tsVarType::tsNone;
				return a;
			}
		};
		class tsAddOperation : public tsDDOperator
		{
		public:
			unsigned int getPriority()
			{
				return 2;
			}
			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of add operation" << std::endl;
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();
				tsVarType type = a.type;


				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

				tsVar result = vars.requestTempVar(type);
				switch (type)
				{
					case tsVarType::tsFloat:
						a = vars.cast<tsVarType::tsFloat>(bytecode, a, line);
						b = vars.cast<tsVarType::tsFloat>(bytecode, b, line);
						bytecode.pushCmd(tsADDF, a.index, b.index, result.index);
						break;
					case tsVarType::tsInt:
						a = vars.cast<tsVarType::tsInt>(bytecode, a, line);
						b = vars.cast<tsVarType::tsInt>(bytecode, b, line);
						bytecode.pushCmd(tsADDI, a.index, b.index, result.index);
						break;
					default:
						throw tsCompileError("Invalid type for add operation", line);
				}
				return result;
			}
		};
		class tsNegateOperation : public tsROperator
		{
			unsigned int getPriority()
			{
				return 4;
			}
			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Writing negate code\n";
				vars.enterScope();
				tsVar a = _dep->getValue(bytecode, vars);
				vars.exitScope();
				tsVarType type = a.type;

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);

				tsVar result = vars.requestTempVar(type);
				switch (type)
				{
					case tsVarType::tsFloat:
						bytecode.pushCmd(tsFLIPF, a.index, result.index);
						break;
					case tsVarType::tsInt:
						bytecode.pushCmd(tsFLIPI, a.index, result.index);
						break;
					default:
						throw tsCompileError("Invalid type for negate operation", line);
				}
				return result;
			}
		};
		class tsSubtractOperation : public tsDDOperator
		{
		public:
			unsigned int getPriority()
			{
				return 2;
			}
			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of subtract operation" << std::endl;
				tsMASSERT((_dep1.get() != nullptr) && (_dep2.get() != nullptr), "Dependancy pointer(s) were null " + std::to_string(_dep1.get() != nullptr) + " " + std::to_string(_dep2.get() != nullptr));
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();
				tsVarType type = GetStdEqType(a, b);

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

				tsVar result = vars.requestTempVar(type);
				switch (type)
				{
					case tsVarType::tsFloat:
						a = vars.cast<tsVarType::tsFloat>(bytecode, a, line);
						b = vars.cast<tsVarType::tsFloat>(bytecode, b, line);
						bytecode.pushCmd(tsFLIPF, b.index, b.index);
						bytecode.pushCmd(tsADDF, a.index, b.index, result.index);
						break;
					case tsVarType::tsInt:
						a = vars.cast<tsVarType::tsInt>(bytecode, a, line);
						b = vars.cast<tsVarType::tsInt>(bytecode, b, line);
						bytecode.pushCmd(tsFLIPI, b.index, b.index);
						bytecode.pushCmd(tsADDI, a.index, b.index, result.index);
						break;
					default:
						throw tsCompileError("Invalid type for subtract operation.", line);
				}
				return result;
			}
		};
		class tsMultiplyOperation : public tsDDOperator
		{
			unsigned int getPriority()
			{
				return 3;
			}
			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of multiply operation" << std::endl;
				tsMASSERT((_dep1.get() != nullptr) && (_dep2.get() != nullptr), "Dependancy pointer(s) were null " + std::to_string(_dep1.get() != nullptr) + " " + std::to_string(_dep2.get() != nullptr));
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();
				tsVarType type = GetStdEqType(a, b);

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

				tsVar result = vars.requestTempVar(type);
				switch (type)
				{
					case tsVarType::tsFloat:
						a = vars.cast<tsVarType::tsFloat>(bytecode, a, line);
						b = vars.cast<tsVarType::tsFloat>(bytecode, b, line);
						bytecode.pushCmd(tsMULF, a.index, b.index, result.index);
						break;
					case tsVarType::tsInt:
						a = vars.cast<tsVarType::tsInt>(bytecode, a, line);
						b = vars.cast<tsVarType::tsInt>(bytecode, b, line);
						bytecode.pushCmd(tsMULI, a.index, b.index, result.index);
						break;
					default:
						throw tsCompileError("Invalid type for multiply operation.", line);
				}
				return result;
			}
		};
		class tsDivideOperation : public tsDDOperator
		{
			unsigned int getPriority()
			{
				return 3;
			}

			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of divide operation" << std::endl;
				tsMASSERT((_dep1.get() != nullptr) && (_dep2.get() != nullptr), "Dependancy pointer(s) were null " + std::to_string(_dep1.get() != nullptr) + " " + std::to_string(_dep2.get() != nullptr));
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();
				tsVarType type = GetStdEqType(a, b);

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

				tsVar result = vars.requestTempVar(type);
				switch (type)
				{
					case tsVarType::tsFloat:
						a = vars.cast<tsVarType::tsFloat>(bytecode, a, line);
						b = vars.cast<tsVarType::tsFloat>(bytecode, b, line);
						bytecode.pushCmd(tsADDF,a.index, b.index, result.index);
						break;
					case tsVarType::tsInt:
						a = vars.cast<tsVarType::tsInt>(bytecode, a, line);
						b = vars.cast<tsVarType::tsInt>(bytecode, b, line);
						bytecode.pushCmd(tsADDI, a.index, b.index, result.index);
						break;
					default:
						throw tsCompileError("Invalid type for divide operation.", line);
				}
				return result;
			}
		};
		class tsNotOperation : public tsROperator
		{
		public:
			unsigned int getPriority()
			{
				return 4;
			}
			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Writing negate code\n";
				vars.enterScope();
				tsVar a = _dep->getValue(bytecode, vars);
				vars.exitScope();
				tsVarType type = a.type;

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (a.type != tsVarType::tsBool)
					throw tsCompileError("Invalid type for not operation: " + (std::string)getVarTypeName(a.type), line);

				tsVar result = vars.requestTempVar(type);
				bytecode.pushCmd(tsNOT, a.index, result.index);
				return result;
			}
		};
		class tsAndOperation : public tsDDOperator
		{
			unsigned int getPriority()
			{
				return 3;
			}

			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of divide operation" << std::endl;
				tsMASSERT((_dep1.get() != nullptr) && (_dep2.get() != nullptr), "Dependancy pointer(s) were null " + std::to_string(_dep1.get() != nullptr) + " " + std::to_string(_dep2.get() != nullptr));
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();
				tsVarType type = GetStdEqType(a, b);

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);
				if (a.type != tsVarType::tsBool)
					throw tsCompileError("Invalid type on left side of AND operation: ", line);
				if (a.type != tsVarType::tsBool)
					throw tsCompileError("Invalid type on right side of AND operation: ", line);

				tsVar result = vars.requestTempVar(type);
				bytecode.pushCmd(tsAND, a.index, b.index, result.index);
				return result;
			}
		};
		class tsOrOperation : public tsDDOperator
		{
			unsigned int getPriority()
			{
				return 3;
			}

			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of divide operation" << std::endl;
				tsMASSERT((_dep1.get() != nullptr) && (_dep2.get() != nullptr), "Dependancy pointer(s) were null " + std::to_string(_dep1.get() != nullptr) + " " + std::to_string(_dep2.get() != nullptr));
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();
				tsVarType type = GetStdEqType(a, b);

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);
				if (a.type != tsVarType::tsBool)
					throw tsCompileError("Invalid type on left side of OR operation: ", line);
				if (b.type != tsVarType::tsBool)
					throw tsCompileError("Invalid type on right side of OR operation: ", line);

				tsVar result = vars.requestTempVar(type);
				bytecode.pushCmd(tsOR, a.index, b.index, result.index);
				return result;
			}
		};
		#pragma region Comparason Operations
		class tsEqualOperation : public tsDDOperator
		{
		public:
			unsigned int getPriority()
			{
				return 3;
			}

			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of equal operation" << std::endl;
				tsMASSERT((_dep1.get() != nullptr) && (_dep2.get() != nullptr), "Dependancy pointer(s) were null " + std::to_string(_dep1.get() != nullptr) + " " + std::to_string(_dep2.get() != nullptr));
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

				tsVarType type;

				if (a.type == tsVarType::tsFloat || b.type == tsVarType::tsFloat)
					type = tsVarType::tsFloat;
				else if (a.type == tsVarType::tsInt && b.type == tsVarType::tsInt)
					type = tsVarType::tsInt;
				else if (a.type == tsVarType::tsBool && b.type == tsVarType::tsBool)
					type = tsVarType::tsBool;
				else
					throw tsCompileError("Invalid comparason operation between" + (std::string)getVarTypeName(a.type) + " and " + (std::string)getVarTypeName(b.type), line);


				tsVar result = vars.requestTempVar(tsVarType::tsBool);
				switch (type)
				{
					case tsVarType::tsFloat:
						a = vars.cast<tsVarType::tsFloat>(bytecode, a, line);
						b = vars.cast<tsVarType::tsFloat>(bytecode, b, line);
						bytecode.pushCmd(tsEqualF, a.index, b.index, result.index);
						break;
					case tsVarType::tsInt:
						bytecode.pushCmd(tsEqualI, a.index, b.index, result.index);
						break;
					case tsVarType::tsBool:
						bytecode.pushCmd(tsEqualB, a.index, b.index, result.index);
				}
				return result;
			}
		};
		class tsLessOperation : public tsDDOperator
		{
		public:
			unsigned int getPriority()
			{
				return 3;
			}

			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of divide operation" << std::endl;
				tsMASSERT((_dep1.get() != nullptr) && (_dep2.get() != nullptr), "Dependancy pointer(s) were null " + std::to_string(_dep1.get() != nullptr) + " " + std::to_string(_dep2.get() != nullptr));
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

				tsVarType type;

				if (a.type == tsVarType::tsFloat || b.type == tsVarType::tsFloat)
					type = tsVarType::tsFloat;
				else if (a.type == tsVarType::tsInt && b.type == tsVarType::tsInt)
					type = tsVarType::tsInt;
				else
					throw tsCompileError("Invalid comparason operation between" + (std::string)getVarTypeName(a.type) + " and " + (std::string)getVarTypeName(b.type), line);


				tsVar result = vars.requestTempVar(tsVarType::tsBool);
				switch (type)
				{
					case tsVarType::tsFloat:
						a = vars.cast<tsVarType::tsFloat>(bytecode, a, line);
						b = vars.cast<tsVarType::tsFloat>(bytecode, b, line);
						bytecode.pushCmd(tsLessF, a.index, b.index, result.index);
						break;
					case tsVarType::tsInt:
						bytecode.pushCmd(tsLessI, a.index, b.index, result.index);
						break;
				}
				return result;
			}
		};
		class tsMoreOperation : public tsDDOperator
		{
		public:
			unsigned int getPriority()
			{
				return 3;
			}

			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of divide operation" << std::endl;
				tsMASSERT((_dep1.get() != nullptr) && (_dep2.get() != nullptr), "Dependancy pointer(s) were null " + std::to_string(_dep1.get() != nullptr) + " " + std::to_string(_dep2.get() != nullptr));
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

				tsVarType type;

				if (a.type == tsVarType::tsFloat || b.type == tsVarType::tsFloat)
					type = tsVarType::tsFloat;
				else if (a.type == tsVarType::tsInt && b.type == tsVarType::tsInt)
					type = tsVarType::tsInt;
				else
					throw tsCompileError("Invalid comparason operation between" + (std::string)getVarTypeName(a.type) + " and " + (std::string)getVarTypeName(b.type), line);


				tsVar result = vars.requestTempVar(tsVarType::tsBool);
				switch (type)
				{
					case tsVarType::tsFloat:
						a = vars.cast<tsVarType::tsFloat>(bytecode, a, line);
						b = vars.cast<tsVarType::tsFloat>(bytecode, b, line);
						bytecode.pushCmd(tsLessF, b.index, a.index, result.index);
						break;
					case tsVarType::tsInt:
						bytecode.pushCmd(tsLessI, b.index, a.index, result.index);
						break;
				}
				return result;
			}
		};
		class tsLessEqualOperation : public tsDDOperator
		{
		public:
			unsigned int getPriority()
			{
				return 3;
			}

			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of divide operation" << std::endl;
				tsMASSERT((_dep1.get() != nullptr) && (_dep2.get() != nullptr), "Dependancy pointer(s) were null " + std::to_string(_dep1.get() != nullptr) + " " + std::to_string(_dep2.get() != nullptr));
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

				tsVarType type;

				if (a.type == tsVarType::tsFloat || b.type == tsVarType::tsFloat)
					type = tsVarType::tsFloat;
				else if (a.type == tsVarType::tsInt && b.type == tsVarType::tsInt)
					type = tsVarType::tsInt;
				else
					throw tsCompileError("Invalid comparason operation between" + (std::string)getVarTypeName(a.type) + " and " + (std::string)getVarTypeName(b.type), line);


				tsVar result = vars.requestTempVar(tsVarType::tsBool);
				switch (type)
				{
					case tsVarType::tsFloat:
						a = vars.cast<tsVarType::tsFloat>(bytecode, a, line);
						b = vars.cast<tsVarType::tsFloat>(bytecode, b, line);
						bytecode.pushCmd(tsLessEqualF, a.index, b.index, result.index);
						break;
					case tsVarType::tsInt:
						bytecode.pushCmd(tsLessEqualI, a.index, b.index, result.index);
						break;
				}
				return result;
			}
		};
		class tsMoreEqualOperation : public tsDDOperator
		{
		public:
			unsigned int getPriority()
			{
				return 3;
			}

			tsVar getValue(tsBytecode& bytecode, tsVarPool& vars)
			{
				std::cout << "Getting value of divide operation" << std::endl;
				tsMASSERT((_dep1.get() != nullptr) && (_dep2.get() != nullptr), "Dependancy pointer(s) were null " + std::to_string(_dep1.get() != nullptr) + " " + std::to_string(_dep2.get() != nullptr));
				vars.enterScope();
				tsVar a = _dep1->getValue(bytecode, vars);
				tsVar b = _dep2->getValue(bytecode, vars);
				vars.exitScope();

				if (!a.initalized)
					throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
				if (!b.initalized)
					throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

				tsVarType type;

				if (a.type == tsVarType::tsFloat || b.type == tsVarType::tsFloat)
					type = tsVarType::tsFloat;
				else if (a.type == tsVarType::tsInt && b.type == tsVarType::tsInt)
					type = tsVarType::tsInt;
				else
					throw tsCompileError("Invalid comparason operation between" + (std::string)getVarTypeName(a.type) + " and " + (std::string)getVarTypeName(b.type), line);


				tsVar result = vars.requestTempVar(tsVarType::tsBool);
				switch (type)
				{
					case tsVarType::tsFloat:
						a = vars.cast<tsVarType::tsFloat>(bytecode, a, line);
						b = vars.cast<tsVarType::tsFloat>(bytecode, b, line);
						bytecode.pushCmd(tsLessEqualF, b.index, a.index, result.index);
						break;
					case tsVarType::tsInt:
						bytecode.pushCmd(tsLessEqualI, b.index, a.index, result.index);
						break;
				}
				return result;
			}
		};
		#pragma endregion

	#pragma endregion

	#pragma region Bytecode Generation

		void GenerateBytecode(std::vector<tsToken> tokens, tsScript& script)
		{
			//GenerateConstVars(tokens, script.bytecode);
			for (size_t i = 0; i < tokens.size(); i++)
			{
				//tsMASSERT(tokens[i].type == tsToken::Type::tsReservedWord, "token was not of type reserved word, this means you haven't coded in error checking yet and you forgot how your own language works.");
				//GenerateStatement(i, tokens, script.bytecode);
				/*
				switch (tokens[i].tokenIndex)
				{
					//case GetIndexOfReservedWord("def"):
						
						//GenerateFunction(i, tokens, script);
						//break;
					default:
						
						break;
				}
			}
		}

		void GenerateFunction(size_t& i, std::vector<tsToken> tokens, tsScript& script)
		{
			tsMASSERT(tokens[i].token == "def", "Generate function called on " + tokens[i].token);
			i++;


			//For now we're going to ignore the paramaters of the function, because I'm impataent and want to get something working.
			//GetFunctionParams(++i, tokens, function);
			//GenerateStatement(i, tokens, script.bytecode);

			//script.functions.push_back(function);
		}
		/*
		void GenerateConstVars(std::vector<tsToken>& tokens, tsBytecode& bytecode)
		{
			std::cout << "Generating constants" << std::endl;
			for(size_t token = 0; token < tokens.size(); token++)
			{
				if (tokens[token].type == tsToken::Type::tsIdentifier)
				{
					try
					{
						if (tokens[token].token.find(".") == std::string::npos)
						{
							int value = std::stoi(tokens[token].token); // try to cast string to int, if it fails it is not a int
							std::cout << "Found const int: " << value << std::endl;
							std::string key = "const int " + std::to_string(value);
							tsVar var;
							if(vars.getVarFromIdentifier(key, var))
							{
								std::cout << "Found predifined const int" << std::endl;
							}
							else
							{
								
								std::cout << "Making new const int" << std::endl;
								tsIndex var = vars.requestVar(tsVarType::tsInt, key, true, true).index;
								bytecode.LOAD(var, value);
							}
							tokens[token].token = key;
						}
						else
						{
							float value = std::stof(tokens[token].token); // try to cast string to float, if it fails it is not a float
							std::cout << "Found const float: " << value << std::endl;
							std::string key = "const float " + std::to_string(value);
							tsVar var;
							if (vars.getVarFromIdentifier(key, var))
							{
								std::cout << "Found predifined const float" << std::endl;
							}
							else
							{

								std::cout << "Making new const float" << std::endl;
								tsIndex var = vars.requestVar(tsVarType::tsFloat, key, true, true).index;
								bytecode.LOAD(var, value);
							}
							tokens[token].token = key;
						}
					}
					catch (...)
					{
					}
					
				}
				else if (tokens[token].type == tsToken::Type::tsReservedWord)
				{
					switch (tokens[token].tokenIndex)
					{
						case GetIndexOfReservedWord("true"):
						{
							std::cout << "Found const true: " << std::endl;
							std::string key = "const true";
							tsVar var;
							if (vars.getVarFromIdentifier(key, var))
							{
								std::cout << "Found predifined const true" << std::endl;
							}
							else
							{

								std::cout << "Making new const true" << std::endl;
								tsIndex var = vars.requestVar(tsVarType::tsBool, key, true, true).index;
								bytecode.LOAD(var, true);
							}
							tokens[token].type = tsToken::Type::tsIdentifier;
							tokens[token].token = key;
						}
							break;
						case GetIndexOfReservedWord("false"):
						{
							std::cout << "Found const false: " << std::endl;
							std::string key = "const false";
							tsVar var;
							if (vars.getVarFromIdentifier(key, var))
							{
								std::cout << "Found predifined const false" << std::endl;
							}
							else
							{

								std::cout << "Making new const false" << std::endl;
								tsIndex var = vars.requestVar(tsVarType::tsBool, key, true, true).index;
								bytecode.LOAD(var, false);
							}
							tokens[token].type = tsToken::Type::tsIdentifier;
							tokens[token].token = key;
						}
							break;
					}
				}
			}
			//We signal that we are done loading with a return;
			bytecode.pushCmd(tsEND);
		}
		/*
		void GenerateStatement(size_t& i, const std::vector<tsToken> tokens, tsBytecode& bytecode)
		{
			
			line = tokens[i].line;
			switch (tokens[i].type)
			{
				case tsToken::Type::tsOperator:
					switch (tokens[i].tokenIndex)
					{
						case GetIndexOfOperator("{"):
						{

							//Store how many varibles are in the parent scope
							vars.enterScope();

							//enter a new scope
							std::cout << "Generating {} statement" << std::endl;
							size_t startLine = line;
							i++;
							while (tokens[i].token != "}")
							{
								GenerateStatement(i, tokens, bytecode);
								if (++i == tokens.size())
									throw tsCompileError("Was expecting closing }", startLine);
							}
							//Delete the varible ids from the last scope
							vars.exitScope();
							break;
						}
						case GetIndexOfOperator(";"):
							break;
						default:
							tsMASSERT(false, "Unknown statement operator: " + tokens[i].token);
							//GenerateExpression(0, i, tokens, bytecode);
							break;
					}
					break;
						
				case tsToken::Type::tsReservedWord:
					switch (tokens[i].tokenIndex)
					{
						case GetIndexOfReservedWord("float"):
							GenerateVar(tsVarType::tsFloat, i, tokens, bytecode);
							break;
						case GetIndexOfReservedWord("int"):
							GenerateVar(tsVarType::tsInt, i, tokens, bytecode);
							break;
						case GetIndexOfReservedWord("bool"):
							GenerateVar(tsVarType::tsBool, i, tokens, bytecode);
							break;
						case GetIndexOfReservedWord("end"):
							GenerateEnd(i, tokens, bytecode);
							break;
						case GetIndexOfReservedWord("if"):
							GenerateIf(i, tokens, bytecode);
							break;
						case GetIndexOfReservedWord("while"):
							GenerateWhile(i, tokens, bytecode);
							break;
						default:
							throw tsCompileError("Unknown reserved word: " + tokens[i].token, line);
							//GenerateExpression
							break;
					}
					break;
						
				default:
					GenerateExpression(i, tokens, bytecode);
					break;
			}
			
			
		}

		tsIndex GenerateExpression(size_t& i, const std::vector<tsToken>& tokens, tsBytecode& bytecode)
		{
			std::unique_ptr<tsOperation> operation;
			size_t end = 0;
			while (tokens[i + end].token != ";")
			{
				++end;
				if (tokens.size() <= i + end)
					throw tsCompileError("Expected ; but file ended", tokens[i + end - 1].line);
			}
			std::vector opTokens(tokens.begin() + i, tokens.begin() + i + end);
			std::cout << "Expression: ";
			for (size_t j = 0; j < opTokens.size(); j++)
			{
				std::cout << opTokens[j].token;
			}
			std::cout << std::endl;
			GenerateOperation(opTokens, operation);

			
			tsIndex returnVal = operation->getValue(bytecode, vars).index;
			
			i += end;
			return returnVal;

		}

		void GenerateOperation(const std::vector<tsToken>& tokens, std::unique_ptr<tsOperation>& operation)
		{
			std::vector<std::unique_ptr<tsOperation>> operations;
			
			//	Identifiy vars and operators then put them in a vector.
			for (size_t i = 0; i < tokens.size(); i++)
			{
				tsToken token = tokens[i];
				line = token.line;
				//Check if there was 
				if (token.type == tsToken::Type::tsIdentifier)
				{
					tsVar var;
					tsIndex index;
					if (getIndexOfFunction(token.token, index))
					{
						
					}
					else if (vars.getVarFromIdentifier(token.token, var))
					{
						std::cout << "found var: " << var.identifier << std::endl;
						operations.push_back(std::make_unique<tsVarOperation>(var));
					}
					else
						throw tsCompileError("Unknown Identifier: " + token.token ,line);
				}
				else if (token.type == tsToken::Type::tsOperator)
				{

					switch (tokens[i].tokenIndex)
					{
						case GetIndexOfOperator("("):
						{
							std::cout << "Found scoped operators: ";

							int pCount = 1;
							std::vector<tsToken> subTokens;
							i++;
							while (pCount > 0)
							{
								if (i >= tokens.size())
									throw tsCompileError("Expected closing )", line);
								if (tokens[i].token == "(")
									pCount++;
								else if (tokens[i].token == ")")
									pCount--;

								if (pCount > 0)
								{
									subTokens.push_back(tokens[i]);
									std::cout << tokens[i].token;
									i++;
								}
								
							}
							std::cout << std::endl;
							std::unique_ptr<tsScopeOperation> subExpression = std::make_unique<tsScopeOperation>();
							GenerateOperation(subTokens, subExpression->operation);
							operations.push_back(std::move(subExpression));
						}
							break;
						case GetIndexOfOperator("+"):
							std::cout << "Found +" << std::endl;
							operations.push_back(std::make_unique<tsAddOperation>());
							break;
						case GetIndexOfOperator("-"):
							std::cout << "Found -" << std::endl;

							if(i == 0 || (tokens[i - 1].type == tsToken::Type::tsOperator && tokens[i - 1].token != ")"))
								operations.push_back(std::make_unique<tsNegateOperation>());
							else
								operations.push_back(std::make_unique<tsSubtractOperation>());
							break;
						case GetIndexOfOperator("*"):
							std::cout << "Found *" << std::endl;

							operations.push_back(std::make_unique<tsMultiplyOperation>());
							break;
						case GetIndexOfOperator("/"):
							std::cout << "Found /" << std::endl;

							operations.push_back(std::make_unique<tsDivideOperation>());
							break;
						case GetIndexOfOperator("&&"):
							std::cout << "Found &&" << std::endl;
							operations.push_back(std::make_unique<tsAddOperation>());
							break;
						case GetIndexOfOperator("||"):
							std::cout << "Found ||" << std::endl;
							operations.push_back(std::make_unique<tsOrOperation>());
							break;
						case GetIndexOfOperator("!"):
							std::cout << "Found !" << std::endl;
							operations.push_back(std::make_unique<tsNotOperation>());
							break;
						case GetIndexOfOperator("=="):
							std::cout << "Found ==" << std::endl;
							operations.push_back(std::make_unique<tsEqualOperation>());
							break;
						case GetIndexOfOperator("<"):
							std::cout << "Found <" << std::endl;
							operations.push_back(std::make_unique<tsLessOperation>());
							break;
						case GetIndexOfOperator(">"):
							std::cout << "Found >" << std::endl;
							operations.push_back(std::make_unique<tsMoreOperation>());
							break;
						case GetIndexOfOperator("<="):
							std::cout << "Found <=" << std::endl;
							operations.push_back(std::make_unique<tsLessEqualOperation>());
							break;
						case GetIndexOfOperator(">="):
							std::cout << "Found >=" << std::endl;
							operations.push_back(std::make_unique<tsMoreEqualOperation>());
							break;
						case GetIndexOfOperator("="):
							std::cout << "Found =" << std::endl;

							operations.push_back(std::make_unique<tsAssignOperation>());
							break;
						default:
							throw tsCompileError("Unknown operator: " + tokens[i].token, line);
					}
				}
				else
					throw tsCompileError("Unexpected reserved word in expression: " + tokens[i].token, line);
				if (operations.size() > 0)
				{
					std::cout << "operations size: " << operations.size() << std::endl;
					tsMASSERT(operations.size() > 0, "size less then 1")
					operations[operations.size() - 1]->line = line;
				}
			}
			
			// I want to refactor this, works but it's ugly, and slow.
			for (int priority = 5; priority >= 0; priority--)
			{
				for (size_t i = 0; i < operations.size(); i++)
				{
					if (operations[i] -> getPriority() == priority)
					{
						switch (operations[i] -> getDepSide())
						{
							case tsOperation::DepSide::both:
								std::cout << "Found double sided operator" << std::endl;

								if (0 > i - 1)
									throw tsCompileError("Value for left side of operator not found", line);
								if (operations.size() <= i + 1)
									throw tsCompileError("Value for right side of operator not found", line);

								static_cast<tsDDOperator*>(operations[i].get()) -> setDeps(operations[i - 1], operations[i + 1]);
								
								// We need to erase these pointers as they are no longer assigned
								// Need to erase +1 first since otherwise the index for it would be off
								operations.erase(operations.begin() + i + 1);
								operations.erase(operations.begin() + i - 1);
								// Need to step back because we deleted and index before this one
								i--;
								break;
							case tsOperation::DepSide::right:
								std::cout << "Found right sided operator" << std::endl;

								if (operations.size() <= i + 1)
									throw tsCompileError("Value for right side of operator not found", line);

								static_cast<tsROperator*>(operations[i].get())->setDep(operations[i + 1]);

								operations.erase(operations.begin() + i + 1);
								break;
							case tsOperation::DepSide::left:
								std::cout << "Found left sided operator" << std::endl;

								if (0 > i - 1)
									throw tsCompileError("Value for left side of operator not found", line);

								static_cast<tsROperator*>(operations[i].get())->setDep(operations[i - 1]);

								operations.erase(operations.begin() + i - 1);
								// Need to step back because we deleted and index before this one
								i--;
								break;
						}
					}
				}
			}
			if (operations.size() != 1)
				throw tsCompileError("Unable to evalulate expression", line);
			operation = std::move(operations[0]);
		}

		bool getIndexOfFunction(const std::string& id, tsIndex& index)
		{
			for (size_t i = 0; i < _context->functions.size(); i++)
			{
				if (_context->functions[i].identifier == id)
				{
					index = i;
					return true;
				}
			}
			return false;
		}

		void GenerateIf(size_t& i, std::vector<tsToken> tokens, tsBytecode& bytecode)
		{
			tsASSERT(tokens[i].token == "if");
			if (++i >= tokens.size())
				throw tsCompileError("Expected statment after if", tokens[i - 1].line);
			if (tokens[i].token != "(")
				throw tsCompileError("Expected \"(\" after if.", tokens[i].line);

			line = tokens[i].line;
			++i;
			int pCount = 1;
			std::vector<tsToken> subTokens;
			std::cout << "If condition: ";
			while (pCount > 0)
			{

				if (i >= tokens.size())
					throw tsCompileError("Expected closing )", line);
				if (tokens[i].token == "(")
					pCount++;
				else if (tokens[i].token == ")")
					pCount--;

				if (pCount > 0)
				{
					subTokens.push_back(tokens[i]);
					std::cout << tokens[i].token;
					i++;
				}
			}
			std::cout << std::endl;
			std::unique_ptr<tsOperation> conditionOperation;
			GenerateOperation(subTokens, conditionOperation);
			tsVar condition = conditionOperation->getValue(bytecode, vars);

			if (condition.type != tsVarType::tsBool)
				throw tsCompileError("condition of if statment must be bool", line);
			if (!condition.initalized)
				throw tsCompileError("condition of if statment must be initalized", line);

			size_t index = bytecode.JUMPF(condition.index);
			GenerateStatement(++i, tokens, bytecode);
			bytecode.bytes.set(index, bytecode.bytes.size());
		}

		void GenerateWhile(size_t& i, std::vector<tsToken> tokens, tsBytecode& bytecode)
		{
			tsASSERT(tokens[i].token == "while");
			if (++i >= tokens.size())
				throw tsCompileError("Expected statment after while", tokens[i - 1].line);
			if (tokens[i].token != "(")
				throw tsCompileError("Expected \"(\" after if.", tokens[i].line);
			line = tokens[i].line;
			++i;
			int pCount = 1;
			std::vector<tsToken> subTokens;
			std::cout << "While condition: ";
			while (pCount > 0)
			{

				if (i >= tokens.size())
					throw tsCompileError("Expected closing )", line);
				if (tokens[i].token == "(")
					pCount++;
				else if (tokens[i].token == ")")
					pCount--;

				if (pCount > 0)
				{
					subTokens.push_back(tokens[i]);
					std::cout << tokens[i].token;
					i++;
				}
			}
			std::cout << std::endl;
			size_t startIndex = bytecode.bytes.size();

			std::unique_ptr<tsOperation> conditionOperation;
			GenerateOperation(subTokens, conditionOperation);
			tsVar condition = conditionOperation->getValue(bytecode, vars);

			if (condition.type != tsVarType::tsBool)
				throw tsCompileError("condition of if statment must be bool", line);
			if (!condition.initalized)
				throw tsCompileError("condition of if statment must be initalized", line);

			size_t endIndex = bytecode.JUMPF(condition.index);
			i ++;
			GenerateStatement(i, tokens, bytecode);
			
			bytecode.GOTO(startIndex);
			bytecode.bytes.set(endIndex, bytecode.bytes.size());
			std::cout << "While start index: " << startIndex << std::endl;
			std::cout << "While end index: " << bytecode.bytes.read<size_t>( endIndex) << std::endl;
			/*
			operation
			if operation jump to end
			statment
			goto operation
			end
			
		}
		
		void GenerateVar(tsVarType type, size_t& i, std::vector<tsToken> tokens, tsBytecode& bytecode)
		{
			if (tokens.size() <= ++i)
				throw tsCompileError("Was expecting identifier", line);
			vars.requestVar(type, tokens[i].token, false);
			if (tokens.size() <= ++i)
				throw tsCompileError("Was expecting ; or expression after variable declaration", line);
			if (tokens[i].token != ";")
				GenerateExpression(--i, tokens, bytecode);
			else
				++i;
		}
		

		void GenerateEnd(size_t& i, std::vector<tsToken> tokens, tsBytecode& bytecode)
		{
			tsASSERT(tokens[i].token == "end");
			bytecode.pushCmd(tsEND);
		}
		*/
	#pragma endregion

	};

	

}

#endif // !TS_COMPILER