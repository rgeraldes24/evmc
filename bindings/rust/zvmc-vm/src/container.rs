// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

use crate::ZvmcVm;

use std::ops::{Deref, DerefMut};

/// Container struct for ZVMC instances and user-defined data.
pub struct ZvmcContainer<T>
where
    T: ZvmcVm + Sized,
{
    #[allow(dead_code)]
    instance: ::zvmc_sys::zvmc_vm,
    vm: T,
}

impl<T> ZvmcContainer<T>
where
    T: ZvmcVm + Sized,
{
    /// Basic constructor.
    pub fn new(_instance: ::zvmc_sys::zvmc_vm) -> Box<Self> {
        Box::new(Self {
            instance: _instance,
            vm: T::init(),
        })
    }

    /// Take ownership of the given pointer and return a box.
    ///
    /// # Safety
    /// This function expects a valid instance to be passed.
    pub unsafe fn from_ffi_pointer(instance: *mut ::zvmc_sys::zvmc_vm) -> Box<Self> {
        assert!(!instance.is_null(), "from_ffi_pointer received NULL");
        Box::from_raw(instance as *mut ZvmcContainer<T>)
    }

    /// Convert boxed self into an FFI pointer, surrendering ownership of the heap data.
    ///
    /// # Safety
    /// This function will return a valid instance pointer.
    pub unsafe fn into_ffi_pointer(boxed: Box<Self>) -> *mut ::zvmc_sys::zvmc_vm {
        Box::into_raw(boxed) as *mut ::zvmc_sys::zvmc_vm
    }
}

impl<T> Deref for ZvmcContainer<T>
where
    T: ZvmcVm,
{
    type Target = T;

    fn deref(&self) -> &Self::Target {
        &self.vm
    }
}

impl<T> DerefMut for ZvmcContainer<T>
where
    T: ZvmcVm,
{
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.vm
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::types::*;
    use crate::{ExecutionContext, ExecutionMessage, ExecutionResult};

    struct TestVm {}

    impl ZvmcVm for TestVm {
        fn init() -> Self {
            TestVm {}
        }
        fn execute(
            &self,
            _revision: zvmc_sys::zvmc_revision,
            _code: &[u8],
            _message: &ExecutionMessage,
            _context: Option<&mut ExecutionContext>,
        ) -> ExecutionResult {
            ExecutionResult::failure()
        }
    }

    unsafe extern "C" fn get_dummy_tx_context(
        _context: *mut zvmc_sys::zvmc_host_context,
    ) -> zvmc_sys::zvmc_tx_context {
        zvmc_sys::zvmc_tx_context {
            tx_gas_price: Uint256::default(),
            tx_origin: Address::default(),
            block_coinbase: Address::default(),
            block_number: 0,
            block_timestamp: 0,
            block_gas_limit: 0,
            block_prev_randao: Uint256::default(),
            chain_id: Uint256::default(),
            block_base_fee: Uint256::default(),
        }
    }

    #[test]
    fn container_new() {
        let instance = ::zvmc_sys::zvmc_vm {
            abi_version: ::zvmc_sys::ZVMC_ABI_VERSION as i32,
            name: std::ptr::null(),
            version: std::ptr::null(),
            destroy: None,
            execute: None,
            get_capabilities: None,
            set_option: None,
        };

        let code = [0u8; 0];

        let message = ::zvmc_sys::zvmc_message {
            kind: ::zvmc_sys::zvmc_call_kind::ZVMC_CALL,
            flags: 0,
            depth: 0,
            gas: 0,
            recipient: ::zvmc_sys::zvmc_address::default(),
            sender: ::zvmc_sys::zvmc_address::default(),
            input_data: std::ptr::null(),
            input_size: 0,
            value: ::zvmc_sys::zvmc_uint256be::default(),
            create2_salt: ::zvmc_sys::zvmc_bytes32::default(),
            code_address: ::zvmc_sys::zvmc_address::default(),
        };
        let message: ExecutionMessage = (&message).into();

        let host = ::zvmc_sys::zvmc_host_interface {
            account_exists: None,
            get_storage: None,
            set_storage: None,
            get_balance: None,
            get_code_size: None,
            get_code_hash: None,
            copy_code: None,
            call: None,
            get_tx_context: Some(get_dummy_tx_context),
            get_block_hash: None,
            emit_log: None,
            access_account: None,
            access_storage: None,
        };
        let host_context = std::ptr::null_mut();

        let mut context = ExecutionContext::new(&host, host_context);
        let container = ZvmcContainer::<TestVm>::new(instance);
        assert_eq!(
            container
                .execute(
                    zvmc_sys::zvmc_revision::ZVMC_SHANGHAI,
                    &code,
                    &message,
                    Some(&mut context)
                )
                .status_code(),
            ::zvmc_sys::zvmc_status_code::ZVMC_FAILURE
        );

        let ptr = unsafe { ZvmcContainer::into_ffi_pointer(container) };

        let mut context = ExecutionContext::new(&host, host_context);
        let container = unsafe { ZvmcContainer::<TestVm>::from_ffi_pointer(ptr) };
        assert_eq!(
            container
                .execute(
                    zvmc_sys::zvmc_revision::ZVMC_SHANGHAI,
                    &code,
                    &message,
                    Some(&mut context)
                )
                .status_code(),
            ::zvmc_sys::zvmc_status_code::ZVMC_FAILURE
        );
    }
}
