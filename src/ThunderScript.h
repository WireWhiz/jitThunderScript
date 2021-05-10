#pragma once
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <any>
#include <memory>
#include "tsMassert.h"
#include "asmjit/x86.h"

using namespace asmjit;

namespace ts
{
	const std::string tsVersion = "0.0.0";


	//bytecode commands:

	#define tsEND        (tsByte)(0) // return values
	#define tsJUMP       (tsByte)(1) // goto a command index
	#define tsJUMPF		 (tsByte)(2) // if a bool is false, goto an index
	#define tsItoF       (tsByte)(3) // cast an int to a float  
	#define tsFtoI       (tsByte)(4) // cast a float to an int
	#define tsLOAD       (tsByte)(5) // load bytes from code into memory
	#define tsMOVE       (tsByte)(6) // copy a set of bytes from one index to another
	#define tsFLIPI      (tsByte)(7) // flip an int
	#define tsADDI       (tsByte)(8) // add an int
	#define tsMULI       (tsByte)(9) // multiply an int
	#define tsDIVI       (tsByte)(10) // divide an int
	#define tsFLIPF      (tsByte)(11) // flip a float
	#define tsADDF       (tsByte)(12) // add a float
	#define tsMULF       (tsByte)(13) // multiply a float
	#define tsDIVF       (tsByte)(14) // divide a float
	#define tsNOT        (tsByte)(15) // invert a bool
	#define tsAND        (tsByte)(16) // and operation on two bools
	#define tsOR         (tsByte)(17) // or operation on two bools
	#define tsLessI      (tsByte)(18) // float less then operation
	#define tsLessF      (tsByte)(19) // int less then operation
	#define tsLessEqualI (tsByte)(20) // float less then or equal to operation
	#define tsLessEqualF (tsByte)(21) // int less then or equal to operation
	#define tsEqualI     (tsByte)(22) // Compare ints
	#define tsEqualF     (tsByte)(23) // Compare ints
	#define tsEqualB     (tsByte)(24) // XAND 

	// Define types for all types used by runtime, so they can be changed if needed, 
	// espesially if diferent platforms have different varible sizes that could break the bytecode.
	typedef unsigned int tsIndex;
	typedef unsigned char tsByte;
	typedef std::int32_t tsInt;
	typedef float tsFloat;
	typedef bool tsBool;
	

	enum class tsVarType
	{
		tsNone,
		tsInt,
		tsFloat,
		tsBool
	};

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

	constexpr const char*  getVarTypeName(tsVarType type)
	{
		switch (type)
		{
			case tsVarType::tsFloat:
				return "float";
			case tsVarType::tsInt:
				return "int";
			case tsVarType::tsBool:
				return "bool";
			default:
				return "Unknown type";
		}
	}

	class tsBytes
	{
	private:
		std::vector<tsByte> bytes;
	public:

		template <class T>
		void pushBack(T value)//, tsIndex index)
		{
			size_t index = bytes.size();

			bytes.resize(bytes.size() + sizeof(T), tsByte(0));
			std::memcpy(&bytes[index], &value, sizeof(T));
		}
		template <class T>
		void set(size_t index, T value)
		{
			tsMASSERT(index >= 0 && index < bytes.size() + 1 - sizeof(T), "Index " + std::to_string(index) + " out of byte range");
			std::memcpy(&bytes[index], &value, sizeof(T));
		}
		template <class T>
		T read(size_t index) const
		{
			tsMASSERT(index >= 0 && index < bytes.size() + 1 - sizeof(T), "Index " + std::to_string(index) + " out of byte range");
			T rv;
			std::memcpy(&rv, &bytes[index], sizeof(T));
			return rv;
		}
		size_t size() const
		{
			return bytes.size();
		}
		void clear()
		{
			bytes.clear();
		}
		void setSize(size_t size)
		{
			bytes.reserve(size);
			bytes.resize(size, tsByte(0));
		}
		void copy(tsIndex a, tsIndex b, size_t size)
		{
			tsMASSERT(a >= 0 && a <= bytes.size() - size + 1, "Index " + std::to_string(a) + " out of byte range");
			tsMASSERT(b >= 0 && b <= bytes.size() - size + 1, "Index " + std::to_string(b) + " out of byte range");
			std::memcpy(&bytes[a], &bytes[b], size);
		}
		void copy(tsBytes& target, tsIndex a, tsIndex b, size_t size) const
		{
			tsMASSERT(a >= 0 && a <= target.bytes.size() - size + 1, "Index " + std::to_string(a) + " out of byte range");
			tsMASSERT(b >= 0 && b <= bytes.size() - size + 1, "Index " + std::to_string(b) + " out of byte range");
			std::memcpy(&target.bytes[a], &bytes[b], size);

		}
	};

	class tsBytecode
	{
		
		std::stack<unsigned int> scope;
		unsigned int numVars = 0;
	public:
		tsBytes bytes;
		
		template<class T>
		void LOAD(tsIndex index, T value)
		{
			bytes.pushBack(tsLOAD);
			bytes.pushBack((unsigned int)sizeof(T));
			bytes.pushBack(index);
			bytes.pushBack(value);
		}
		void MOVE(tsIndex var, tsIndex targetIndex, unsigned int size)
		{
			bytes.pushBack(tsMOVE);
			bytes.pushBack(size);
			bytes.pushBack(var);
			bytes.pushBack(targetIndex);
		}
		template<tsVarType T1, tsVarType T2>
		tsIndex JUMPF(tsIndex condition)
		{
			bytes.pushBack(tsJUMPF);
			bytes.pushBack(condition);
			size_t gotoIndex = bytes.size();
			bytes.pushBack((tsIndex)(0));
			return gotoIndex;
		}
		void GOTO(tsIndex index)
		{
			bytes.pushBack(tsJUMP);
			bytes.pushBack(index);
		}
		void pushCmd(tsByte c)
		{
			bytes.pushBack(c);
		}
		void pushCmd(tsByte c, tsIndex i)
		{
			bytes.pushBack(c);
			bytes.pushBack(i);
		}
		void pushCmd(tsByte c, tsIndex i, tsIndex r)
		{
			bytes.pushBack(c);
			bytes.pushBack(i);
			bytes.pushBack(r);
		}
		void pushCmd(tsByte c, tsIndex a, tsIndex b, tsIndex r)
		{
			bytes.pushBack(c);
			bytes.pushBack(a);
			bytes.pushBack(b);
			bytes.pushBack(r);
		}
	};


	class tsGlobal
	{
	public:
		enum class GlobalType
		{
			tsIn,
			tsRef
		};

		tsVarType type;
		GlobalType writeMode;
		std::string identifier;
		tsIndex index;
	};

	class tsConst
	{
	public:
		tsVarType type;
		std::string identifier;
		tsIndex index;
	};

	class tsFunction
	{
	public:
		std::string identifier;
		           //Index          bytecode                 stack
		void (*func)(tsIndex, const std::vector<tsByte>&, std::vector<tsByte>&);
	};

	class tsScript
	{
	public:
		tsIndex numBytes;

		std::vector<tsGlobal> globals;
		tsBytecode bytecode;
	};
	

	class tsContext
	{
	public:
		std::vector<tsFunction> functions;
		std::vector<tsScript> scripts;
	};

	class tsRuntime
	{
	private:
		std::shared_ptr<tsContext> _context;

		tsIndex loadedScript;
		bool scriptLoaded;

		size_t cursor;
		tsBytes stack;

	public:
		tsRuntime(std::shared_ptr<tsContext>& context)
		{
			_context = context;
		}

		void LoadScript(tsIndex script)
		{
			loadedScript = script;
			cursor = 0;
			stack.clear();
			stack.setSize(_context->scripts[loadedScript].numBytes);
			//CompileByteCode(_context->scripts[loadedScript].bytecode);
			scriptLoaded = true;
		}


		template<class T>
		void SetGlobal(std::string identifier, T value)
		{
			if (scriptLoaded)
			{
				tsScript& script = _context->scripts[loadedScript];
				for (tsIndex i = 0; i < script.globals.size(); i++)
				{
					if (script.globals[i].identifier == identifier)
					{
						SetGlobal(i, value);
						return;
					}
				}
				tsMASSERT(false, "Could not find identifier: " + identifier);
			}
			else
				std::cout << "Could not set global, no loaded function.\n";
		}

		template<class T>
		void SetGlobal(tsIndex index, T value)
		{
			tsScript& script = _context->scripts[loadedScript];
			stack.set(script.globals[index].index, value);
			std::cout << "Global " << index << " at index " << script.globals[index].index << " set to " << stack.read<T>(script.globals[index].index) << std::endl;
		}

		template<class T>
		T GetGlobal(std::string identifier)
		{
			if (scriptLoaded)
			{
				tsScript& script = _context->scripts[loadedScript];
				for (tsIndex i = 0; i < script.globals.size(); i++)
				{
					if (script.globals[i].identifier == identifier)
					{
						return GetGlobal<T>(i);
					}
				}
				tsMASSERT(false, "Could not find identifier: " + identifier);
			}
			else
				std::cout << "Could not set global, no loaded function.\n";
			return 0;
		}

		template<class T>
		T GetGlobal(tsIndex index)
		{
			tsScript& script = _context->scripts[loadedScript];
			return stack.read<T>(script.globals[index].index);
		}

		void Run()
		{
			std::cout << "Running script: " << loadedScript << " at cursor index: " << cursor << std::endl;
			size_t start = cursor;
			CompileByteCode(_context->scripts[loadedScript].bytecode);
			// Return the cursor to the start of the function after we complete it
			cursor = start;
		}

		typedef int (*tsjitFunc)(const tsInt* a, const tsInt* b);
		void CompileByteCode(const tsBytecode& bytecode)
		{
			JitRuntime rt;
			CodeHolder code;

			code.init(rt.environment());
			x86::Compiler cc(&code);
			auto funcSign = FuncSignatureT<tsInt, const tsInt*, const tsInt*>(CallConv::kIdHost);
			cc.addFunc(funcSign);
			std::cout << "Function arguments: " << funcSign.argCount() << std::endl;
			x86::Gp a = cc.newIntPtr("a");
			x86::Gp b = cc.newIntPtr("b");

			cc.setArg(0, a);
			cc.setArg(1, b);
			x86::Gp tmp = cc.newInt32("tmp");
			cc.mov(tmp, x86::dword_ptr(a));
			cc.add(tmp, x86::dword_ptr(b));
			cc.ret(tmp);

			cc.endFunc();
			cc.finalize();
			tsjitFunc fn;
			Error err = rt.add(&fn, &code);
			if (err)
			{
				throw tsCompileError("Function could not be compiled", 0);
			}
			
			bool executing = true;
			while (executing && cursor < bytecode.bytes.size())
			{
				//std::cout << "Executing code " << (int)bytecode.bytes.read<tsByte>(cursor) << " at index " << cursor << std::endl;
				switch (bytecode.bytes.read<tsByte>(cursor))
				{
					case tsEND:
						executing = false;
						break;
					case tsJUMP:
					{
						size_t index = bytecode.bytes.read<size_t>(++cursor);
						cursor = index - 1;
					}
						break;
					case tsJUMPF:
					{
						tsIndex condition = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						size_t index = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						if (!stack.read<tsBool>(condition))
						{
							cursor = index;
						}
						break;
					}
					case tsLOAD:
					{
						size_t size = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex index = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						bytecode.bytes.copy(stack, index, cursor, size);
						cursor += size;
					}
						break;
					case tsMOVE:
					{
						tsIndex size = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex index1 = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						tsIndex index2 = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.copy(index2, index1, size);
					}
						break;
					case tsFtoI:
					{
						tsIndex index1 = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						stack.set<tsInt>(index1, stack.read<tsFloat>(index1));
					}
						break;
					case tsItoF:
					{
						tsIndex index1 = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						stack.set<tsFloat>(index1, stack.read<tsInt>(index1));
					}
						break;
					case tsFLIPF:
					{
						tsIndex index1 = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						stack.set(index1, -stack.read<tsFloat>(index1));
					}
						break;
					case tsADDF:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set(a, stack.read<tsFloat>(a) + stack.read<tsFloat>(b));
						break;
					}
					case tsMULF:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set(a, stack.read<tsFloat>(a) * stack.read<tsFloat>(b));
					}
						break;
					case tsDIVF:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set(a, stack.read<tsFloat>(a) / stack.read<tsFloat>(b));
					}
					break;
					case tsFLIPI:
					{
						tsIndex index1 = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex index2 = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set(index2, -stack.read<tsInt>(index1));
					}
					break;
					case tsADDI:
					{
						tsIndex aIndex = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex bIndex = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						std::cout << "adding " << stack.read<tsInt>(aIndex) << " + " << stack.read<tsInt>(bIndex) << std::endl;
						// Test of jit compiled function
						int a = stack.read<tsInt>(aIndex);
						int b = stack.read<tsInt>(bIndex);
						stack.set(aIndex, fn(&a, &b));
						break;
					}
					case tsMULI:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor); 
						cursor += sizeof(tsIndex);
						stack.set(a, stack.read<tsInt>(a) * stack.read<tsInt>(b));
					}
					break;
					case tsDIVI:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set(a, stack.read<tsInt>(a) / stack.read<tsInt>(b));
					}
					break;
					case tsNOT:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex r = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set(r, !stack.read<tsBool>(a));
					}
					break;
					case tsAND:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						tsIndex r = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set(r, stack.read<tsBool>(a) && stack.read<tsBool>(b));
					}
					break;
					case tsOR:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						tsIndex r = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set(r, stack.read<tsBool>(a) || stack.read<tsBool>(b));
					}
					break;
					case tsEqualI:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						tsIndex r = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set<tsBool>(r, stack.read<tsInt>(a) == stack.read<tsInt>(b));
					}
						break;
					case tsEqualF:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						tsIndex r = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set<tsBool>(r, stack.read<tsFloat>(a) == stack.read<tsFloat>(b));
					}
					break;
					case tsEqualB:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						tsIndex r = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set<tsBool>(r, stack.read<tsBool>(a) == stack.read<tsBool>(b));
					}
					break;
					case tsLessI:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						tsIndex r = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set<tsBool>(r, stack.read<tsInt>(a) < stack.read<tsInt>(b));
					}
					break;
					case tsLessF:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						tsIndex r = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set<tsBool>(r, stack.read<tsFloat>(a) < stack.read<tsFloat>(b));
					}
					break;
					case tsLessEqualI:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						tsIndex r = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set<tsBool>(r, stack.read<int>(a) <= stack.read<int>(b));
					}
					break;
					case tsLessEqualF:
					{
						tsIndex a = bytecode.bytes.read<tsIndex>(++cursor);
						cursor += sizeof(tsIndex);
						tsIndex b = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						tsIndex r = bytecode.bytes.read<tsIndex>(cursor);
						cursor += sizeof(tsIndex);
						stack.set<tsBool>(r, stack.read<tsFloat>(a) <= stack.read<tsFloat>(b));
					}
					break;
					default:
					{
					#ifdef _DEBUG
						tsMASSERT(false, "Unknown byte code! " + std::to_string((unsigned int)bytecode.bytes.read<tsByte>(cursor)));
					#else
						//This apperently removes the default check from the switch, increasing speed
					#endif
					}
				}
			}
			
		}
	};
}