#pragma once

#include <App/App.hpp>
#include <EntryPoint/EntryPoint.hpp>

class Sandbox : public drop::CoreApp
{
public:

	Sandbox(const drop::CommandLineArgs& args);

	void SandboxTest();

private:

};
