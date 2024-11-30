// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

use std::collections::HashMap;
use zvmc_declare::zvmc_declare_vm;
use zvmc_vm::ExecutionContext;
use zvmc_vm::ExecutionMessage;
use zvmc_vm::ExecutionResult;
use zvmc_vm::SetOptionError;
use zvmc_vm::ZvmcVm;

#[zvmc_declare_vm("Foo VM", "zwasm, zvm", "1.42-alpha.gamma.starship")]
pub struct FooVM {
    options: HashMap<String, String>,
}

impl ZvmcVm for FooVM {
    fn init() -> Self {
        Self {
            options: Default::default(),
        }
    }

    fn set_option(&mut self, key: &str, value: &str) -> Result<(), SetOptionError> {
        self.options.insert(key.to_string(), value.to_string());

        Ok(())
    }

    fn execute(
        &self,
        _revision: zvmc_sys::zvmc_revision,
        _code: &[u8],
        _message: &ExecutionMessage,
        _context: Option<&mut ExecutionContext>,
    ) -> ExecutionResult {
        ExecutionResult::success(1337, 21, None)
    }
}
