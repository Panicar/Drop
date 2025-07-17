#pragma once

#include <Application/Application.hpp>
#include <EntryPoint/EntryPoint.hpp>

class Sandbox : public drop::CoreApplication
{
public:

	Sandbox(const drop::CommandLineArgs& args);

	void SandboxTest();

private:

};
