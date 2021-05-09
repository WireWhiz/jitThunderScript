#include <vector>
#include <array>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <map>
#include <stack>
#include <unordered_map>

#include "ThunderScript.h"
#include "thunderScriptCompiler.h"
#include "bison/bison.tab.hh"

#if ! defined(yyFlexLexerOnce)
#endif
#include <FlexLexer.h>
#include "location.hh"


namespace ts
{

	typedef std::pair<tsVarType, tsVarType> tsVarTypePair;

	struct tsVarTypePairHash
	{
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const
		{
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

	std::unordered_map<tsVarTypePair, std::byte, tsVarTypePairHash> tsVarCastCodes = {
		{{tsVarType::tsInt, tsVarType::tsFloat}, tsItoF},
		{{tsVarType::tsFloat, tsVarType::tsInt}, tsFtoI}
	};


	tsVarPool::tsVarPool()
	{
		scopes.push(std::vector<tsVar*>());
	}
	void tsVarPool::reset()
	{
		vars.clear();
		bytes = 0;
		tempIndex = 0;
		scopes.empty();
	}
	size_t tsVarPool::sizeOf()
	{
		return bytes;
	}
	void tsVarPool::enterScope()
	{
		std::cout << "entering Scope " << scopes.size() << std::endl;
		scopes.push(std::vector<tsVar*>());
	}
	void tsVarPool::exitScope()
	{
		std::cout << "exiting Scope " << scopes.size() << std::endl;
		for (size_t i = 0; i < scopes.top().size(); i++)
		{
			scopes.top()[i]->inUse = false;
		}
		scopes.pop();
	}
	tsVar tsVarPool::requestTempVar(tsVarType type, size_t line)
	{
		tsVar r = requestVar("temp " + std::to_string(tempIndex++), type, line, false, true);
		assert(r.initalized == true);
		setTemp(r, true);
		return r;
	}

	tsVar tsVarPool::requestVar(const std::string& identifier, tsVarType type, size_t line, bool isConstant, bool isInitalized)
	{
		for (tsIndex i = 0; i < vars.size(); i++)
			if (vars[i].identifier == identifier && vars[i].inUse)
				throw tsCompileError("Variable with identifier: " + identifier + " has already been defined", line);
		int size = 0;
		switch (type)
		{
			case tsVarType::tsInt:
				size = sizeof(tsInt);
				break;
			case tsVarType::tsFloat:
				size = sizeof(tsFloat);
				break;
			case tsVarType::tsBool:
				size = sizeof(tsBool);
				break;
			default:
				tsMASSERT(false, "Tried to create var of unimplemented type");
				break;
		}
		
		for (tsIndex i = 0; i < vars.size(); i++)
		{
			if (!vars[i].inUse && vars[i].size == size)
			{
				vars[i].inUse = true;
				vars[i].isTemp = false;
				vars[i].identifier = identifier;
				vars[i].constant = isConstant;
				vars[i].initalized = isInitalized;
				scopes.top().push_back(&vars[i]);
				std::cout << "Reused var at index: " << vars[i].index << std::endl;
				return vars[i];
			}
		}
		tsVar var;
		
		var.index = bytes;
		var.type = type;
		var.size = size;
		bytes += var.size;
		var.inUse = true;
		var.isTemp = false;
		var.constant = isConstant;
		var.identifier = identifier;
		var.initalized = isInitalized;
		size_t index = vars.size();
		var.varIndex = index;
		vars.push_back(var);
		scopes.top().push_back(&vars[index]);
		std::cout << "created var at byte index: " << var.index << " with size: " << var.size << std::endl;
		return var;
	}

	tsVar tsVarPool::requestInlineConst(const std::string& value, tsVarType type, size_t line)
	{
		std::string constKey = "const " + (std::string)getVarTypeName(type) + " " + value;
		for (tsIndex i = 0; i < vars.size(); i++)
			if (vars[i].inUse && vars[i].identifier == constKey)
			{
				return vars[i];
			}
		tsVar var;
		switch (type)
		{
			case tsVarType::tsInt:
				var.size = sizeof(int);
				break;
			case tsVarType::tsFloat:
				var.size = sizeof(float);
				break;
			case tsVarType::tsBool:
				var.size = sizeof(bool);
				break;
			default:
				tsMASSERT(false, "Tried to create var of unimplemented type");
				break;
		}
		var.index = bytes;
		bytes += var.size;
		var.type = type;
		var.inUse = true;
		var.constant = true;
		var.identifier = constKey;
		var.initalized = false;
		size_t index = vars.size();
		var.varIndex = index;
		vars.push_back(var);
		std::cout << "created inline const var at byte index: " << var.index << std::endl;
		return var;
	}

	void tsVarPool::initialize(tsVar& var)
	{
		for (tsIndex i = 0; i < vars.size(); i++)
		{
			if (vars[i].inUse && var.identifier == vars[i].identifier)
			{
				vars[i].initalized = true;
				var.initalized = true;
				return;
			}
		}
		assert(false);//Could not find var
	}
	void tsVarPool::setTemp(tsVar& var, bool temp)
	{
		vars[var.varIndex].isTemp = temp;
		var.isTemp = temp;

	}
	bool tsVarPool::getVarFromIdentifier(std::string identifier, tsVar& var)
	{
		for (tsIndex i = 0; i < vars.size(); i++)
		{
			if (vars[i].inUse && identifier == vars[i].identifier)
			{
				var = vars[i];
				return true;
			}
		}
		return false;
	}
	tsVar tsVarPool::getVarFromIndex(size_t index)
	{
		assert(index >= 0 && index < vars.size());
		return vars[index];
	}

	tsScanner::tsScanner(std::istream* in) : yyFlexLexer(in)
	{
		loc = new tsParser::location_type();
	};


	tsCompiler::tsCompiler(std::shared_ptr<tsContext>& context)
	{
		_context = context;
	}

	tsCompiler::~tsCompiler()
	{
		delete scanner;
		delete parser;
	}

	bool tsCompiler::compileFile(const std::string& path)
	{
		// Read the script file. Return if we can't find it.
		std::ifstream f;
		f.open(path);
		if (!f.is_open())
			return false;

		//Read the entire script into a string
		char c;
		std::string scriptText = "";
		while (f.get(c))
			scriptText += c;

		// Release the file
		f.close();
		return compile(scriptText);
	}

	bool tsCompiler::compile(std::string& scriptText)
	{
		vars.reset();

		script = std::make_unique<tsScript>();
		removeComments(scriptText);
		std::stringstream scriptStream(scriptText);

		//Create scanner
		delete scanner;
		try
		{
			scanner = new tsScanner(&scriptStream);
		}
		catch (std::bad_alloc& error)
		{
			std::cerr << "Failed to allocate scanner: (" <<
				error.what() << "), exiting!!" << std::endl;
			return false;
		}

		//create parser
		delete(parser);
		try
		{
			parser = new tsParser((*scanner), (*this));
		}
		catch (std::bad_alloc& error)
		{
			std::cout << "Failed to allocate parser: (" <<
				error.what() << "), exiting!!" << std::endl;
			return false;
		}

		//parse
		const int accept(0);
		if (parser->parse() != accept)
		{
			std::cout << "Parse failed" << std::endl;
			return false;
		}
		script->numBytes = vars.sizeOf();
		_context->scripts.push_back(*script);
		return true;
	}

#pragma region PreProcessing
	void tsCompiler::removeComments(std::string& scriptText)
	{
		size_t len = scriptText.length();

		for (size_t i = 0; i < len - 1; i++)
		{

			std::string key({ scriptText[i],scriptText[i + 1] });
			if (key == "//")
			{
				std::cout << "found // comment" << std::endl;
				size_t length = scriptText.find("\n", i);
				while (i < length)
				{
					if (scriptText[i] != '\n')
					{
						scriptText.erase(scriptText.begin() + i);
						length--;
					}
					else
					{
						i++;
					}
				}
				scriptText.insert(i, " ");
				len = scriptText.length();
			}
			else if (key == "/*")
			{
				std::cout << "Found /**/ comment" << std::endl;
				size_t length = scriptText.find("*/", i) + 2;
				while (i < length)
				{
					if (scriptText[i] != '\n')
					{
						scriptText.erase(scriptText.begin() + i);
						length--;
					}
					else
					{
						i++;
					}
				}
				scriptText.insert(i, " ");
				len = scriptText.length();
			}
		}
	}

#pragma endregion


#pragma region BisonFunctions
	void tsCompiler::generateGlobal(const std::string& identifier, const tsVarType& type, const tsGlobal::GlobalType writeMode, const size_t line)
	{
		tsGlobal g;
		g.type = type;
		g.identifier = identifier;
		std::cout << "Creating global: " << g.identifier << std::endl;
		tsVar var = vars.requestVar(g.identifier, g.type, line, tsGlobal::GlobalType::tsIn == writeMode, true);
		g.index = var.index;
		script->globals.push_back(g);
	}

	tsVar tsCompiler::generateVar(const std::string& identifier, const tsVarType& type)
	{
		std::cout << "Making: " << getVarTypeName(type) << std::endl;
		return vars.requestVar(identifier, type, false);
	}

	size_t tsCompiler::getVarIndex(const std::string& identifier)
	{
		tsVar o;
		if (vars.getVarFromIdentifier(identifier, o))
			return o.varIndex;
		assert(false); // We should never reach this
		return o.varIndex;
	}
	tsVar tsCompiler::getVar(const size_t& index)
	{
		return vars.getVarFromIndex(index);
	}

	void tsCompiler::assignVar(size_t ai, size_t bi, size_t line)
	{
		tsVar a = vars.getVarFromIndex(ai);
		tsVar b = vars.getVarFromIndex(bi);
		assignVar(a, b, line);
	}
	void tsCompiler::assignVar(tsVar a, tsVar b, size_t line)
	{
		if (!b.initalized)
			throw tsCompileError("variable " + b.identifier + " not initalized", line);
		if (a.constant)
			throw tsCompileError("can not set constant variable: " + a.identifier, line);

		std::cout << "Assigning " << a.identifier << " to " << b.identifier << std::endl;
		castVar(b, a.type, line);
		vars.initialize(a);
		script->bytecode.MOVE(b.index, a.index, a.size);
	}

	void tsCompiler::castVar(tsVar& var, tsVarType targetType, size_t line)
	{
		if (var.type == targetType)
			return;
		if (!var.initalized)
			throw tsCompileError("variable " + var.identifier + " not initalized", line);
		if (var.type == tsVarType::tsNone)
			throw tsCompileError("Can not cast varible of none type to " + (std::string)getVarTypeName(var.type), line);

		
		
		try
		{
			if (!var.isTemp)
			{
				std::cout << "casing non temp var, creating copy of " << var.identifier << std::endl;
				tsVar newVar = vars.requestTempVar(targetType, line);
				script->bytecode.MOVE(var.index, newVar.index, var.size);
				script->bytecode.pushCmd(tsVarCastCodes.at({ var.type, targetType}), newVar.index);
				var = newVar;
			}
			else
			{
				script->bytecode.pushCmd(tsVarCastCodes.at({ var.type, targetType }), var.index);
				var.type = targetType;
			}
		}
		catch (const std::out_of_range& e)
		{
			throw tsCompileError("Can not cast type: " + (std::string)getVarTypeName(var.type) + " to " + (std::string)getVarTypeName(targetType), line);
		}
	}

	size_t tsCompiler::getConst(const std::string value, tsVarType type, size_t line)
	{
		std::cout << "Found const: " << value << std::endl;
		tsVar tsConst = vars.requestInlineConst(value, type, line);
		if (!tsConst.initalized)
		{
			switch (type)
			{
				case ts::tsVarType::tsInt:
					script->bytecode.LOAD<tsInt>(tsConst.index, std::stoi(value));
					break;
				case ts::tsVarType::tsFloat:
					script->bytecode.LOAD<tsFloat>(tsConst.index, std::stof(value));
					break;
				case ts::tsVarType::tsBool:
					if (value[0] == 't')
						script->bytecode.LOAD<tsBool>(tsConst.index, true);
					else
						script->bytecode.LOAD<tsBool>(tsConst.index, false);
					break;
				default:
					assert(false);
					break;
			}
			vars.initialize(tsConst);
		}
		return tsConst.varIndex;
	}

	void tsCompiler::enterScope()
	{
		vars.enterScope();
	}
	void tsCompiler::exitScope()
	{
		vars.exitScope();
	}

	std::unordered_map<tsVarTypePair, tsVarType, tsVarTypePairHash> mathType =
	{
		{{tsVarType::tsInt, tsVarType::tsInt}, tsVarType::tsInt},
		{{tsVarType::tsFloat, tsVarType::tsFloat}, tsVarType::tsFloat},
		{{tsVarType::tsFloat, tsVarType::tsInt }, tsVarType::tsFloat},
		{{tsVarType::tsInt, tsVarType::tsFloat }, tsVarType::tsFloat},
	};

	size_t tsCompiler::add(size_t ai, size_t bi, size_t line)
	{
		tsVar a = vars.getVarFromIndex(ai);
		tsVar b = vars.getVarFromIndex(bi);

		std::cout << "adding " << a.identifier << " and " << b.identifier << std::endl;

		if (!a.initalized)
			throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
		if (!b.initalized)
			throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

		tsVarType type;
		try
		{
			 type = a.type;
		}
		catch (const std::out_of_range& e)
		{
			throw tsCompileError("Can not add types " + (std::string)getVarTypeName(a.type) + " and " + (std::string)getVarTypeName(b.type), line);
		}
		enterScope();//It's ok to set these varibles as unused as their values will be read before anyone has a chance to overwrite them
		
		castVar(a, type, line);
		castVar(b, type, line);

		if (!a.isTemp)
		{
			std::cout << "Right side of + " << a.identifier << "  is not temp, creating temp for result" << std::endl;
			tsVar result = vars.requestTempVar(type, line);
			script->bytecode.MOVE(a.index, result.index, a.size);
			a = result;
		}
		exitScope();
		std::byte code;
		switch (type)
		{
			case tsVarType::tsFloat:
				code = tsADDF;
				break;
			case tsVarType::tsInt:
				code = tsADDI;
				break;
			default:
				throw tsCompileError("Invalid type for add operation", line);
		}
		script->bytecode.pushCmd(code, a.index, b.index);
		return a.varIndex;
	}
	size_t tsCompiler::sub(size_t ai, size_t bi, size_t line)
	{
		tsVar a = vars.getVarFromIndex(ai);
		tsVar b = vars.getVarFromIndex(bi);
		std::cout << "subtracting " << a.identifier << " and " << b.identifier << std::endl;


		if (!a.initalized)
			throw tsCompileError("Use of uninitalized variable: " + a.identifier, line);
		if (!b.initalized)
			throw tsCompileError("Use of uninitalized varialbe: " + b.identifier, line);

		tsVarType type;
		try
		{
			type = a.type;
		}
		catch (const std::out_of_range& e)
		{
			throw tsCompileError("Can not subtract types " + (std::string)getVarTypeName(a.type) + " and " + (std::string)getVarTypeName(b.type), line);
		}
		enterScope();
		castVar(a, type, line);
		castVar(b, type, line);
		tsVar result = vars.requestTempVar(type, line);
		exitScope();
		std::byte code;
		switch (type)
		{
			case tsVarType::tsFloat:
				code = tsADDF;
				script->bytecode.pushCmd(tsFLIPF, b.index, result.index);
				break;
			case tsVarType::tsInt:
				code = tsADDI;
				script->bytecode.pushCmd(tsFLIPI, b.index, result.index);
				break;
			default:
				throw tsCompileError("Invalid type for add operation", line);
		}
		script->bytecode.pushCmd(code, a.index, result.index, result.index);
		return result.varIndex;
	}

	#pragma endregion
}