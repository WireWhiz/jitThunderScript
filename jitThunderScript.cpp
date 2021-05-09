#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include "src/tsMassert.h"
#include "src/ThunderScript.h"
#include "src/ThunderScriptCompiler.h"
#include "src/TSBytecodeDebugger.h";



int main()
{
	std::cout << "ThunderScript Compiler Version: " << ts::tsVersion << std::endl;

	//while (true)
	{
		//std::cout << "Please enter a file path: ";
		std::string filePath = "scripts/HelloWorld.thun";
		//std::cin >> filePath;
		//if (filePath == "exit")
			//break;


		std::shared_ptr<ts::tsContext> tsc = std::make_shared<ts::tsContext>();
		try
		{

			ts::tsCompiler compiler(tsc);
			if (compiler.compileFile(filePath))
			{
				std::cout << "Sucessfully read file!" << std::endl;
				ts::tsBytecode bytecode = tsc->scripts[0].bytecode;

				ts::DisplayBytecode(bytecode);

				char input;
				std::cout << "Do you want to run it? (y/n): ";
				std::cin >> input;
				if (input == 'y')
				{
					std::cout << "Running script:\n\n";
					ts::tsRuntime runtime(tsc);

					runtime.LoadScript(0);
					runtime.SetGlobal<ts::tsFloat>("a", 2);
					runtime.SetGlobal<ts::tsFloat>("b", 3);
					auto start = std::chrono::high_resolution_clock::now();
					runtime.Run();
					auto stop = std::chrono::high_resolution_clock::now();
					std::cout << "\n\nGlobal c has a value of: " << runtime.GetGlobal<ts::tsFloat>("c") << std::endl;
					std::cout << "Program took: " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()
						<< " microseconds" << std::endl;
				}
			}
			else
			{
				std::cout << "Could not read file." << std::endl;
			}
		}
		catch (ts::tsCompileError error)
		{
			error.display();
		}


	}
	return 0;
}