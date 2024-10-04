#pragma once

#include "Modules/ModuleManager.h"
#include "FactoryGame.h"

class FFicsitWiremodModule : public FDefaultGameModuleImpl {
public:
	virtual void StartupModule() override;

	virtual bool IsGameModule() const override { return true; }
};