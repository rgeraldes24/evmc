use zvmc_sys as ffi;

/// ZVMC address
pub type Address = ffi::zvmc_address;

/// ZVMC 32 bytes value (used for hashes)
pub type Bytes32 = ffi::zvmc_bytes32;

/// ZVMC big-endian 256-bit integer
pub type Uint256 = ffi::zvmc_uint256be;

/// ZVMC call kind.
pub type MessageKind = ffi::zvmc_call_kind;

/// ZVMC message (call) flags.
pub type MessageFlags = ffi::zvmc_flags;

/// ZVMC status code.
pub type StatusCode = ffi::zvmc_status_code;

/// ZVMC access status.
pub type AccessStatus = ffi::zvmc_access_status;

/// ZVMC storage status.
pub type StorageStatus = ffi::zvmc_storage_status;

/// ZVMC VM revision.
pub type Revision = ffi::zvmc_revision;

#[cfg(test)]
mod tests {
    use super::*;

    // These tests check for Default, PartialEq and Clone traits.
    #[test]
    fn address_smoke_test() {
        let a = ffi::zvmc_address::default();
        let b = Address::default();
        assert_eq!(a.clone(), b.clone());
    }

    #[test]
    fn bytes32_smoke_test() {
        let a = ffi::zvmc_bytes32::default();
        let b = Bytes32::default();
        assert_eq!(a.clone(), b.clone());
    }

    #[test]
    fn uint26be_smoke_test() {
        let a = ffi::zvmc_uint256be::default();
        let b = Uint256::default();
        assert_eq!(a.clone(), b.clone());
    }

    #[test]
    fn message_kind() {
        assert_eq!(MessageKind::ZVMC_CALL, ffi::zvmc_call_kind::ZVMC_CALL);
        assert_eq!(
            MessageKind::ZVMC_DELEGATECALL,
            ffi::zvmc_call_kind::ZVMC_DELEGATECALL
        );
        assert_eq!(MessageKind::ZVMC_CREATE, ffi::zvmc_call_kind::ZVMC_CREATE);
    }

    #[test]
    fn message_flags() {
        assert_eq!(MessageFlags::ZVMC_STATIC, ffi::zvmc_flags::ZVMC_STATIC);
    }

    #[test]
    fn status_code() {
        assert_eq!(
            StatusCode::ZVMC_SUCCESS,
            ffi::zvmc_status_code::ZVMC_SUCCESS
        );
        assert_eq!(
            StatusCode::ZVMC_FAILURE,
            ffi::zvmc_status_code::ZVMC_FAILURE
        );
    }

    #[test]
    fn access_status() {
        assert_eq!(
            AccessStatus::ZVMC_ACCESS_COLD,
            ffi::zvmc_access_status::ZVMC_ACCESS_COLD
        );
        assert_eq!(
            AccessStatus::ZVMC_ACCESS_WARM,
            ffi::zvmc_access_status::ZVMC_ACCESS_WARM
        );
    }

    #[test]
    fn storage_status() {
        assert_eq!(
            StorageStatus::ZVMC_STORAGE_ASSIGNED,
            ffi::zvmc_storage_status::ZVMC_STORAGE_ASSIGNED
        );
        assert_eq!(
            StorageStatus::ZVMC_STORAGE_MODIFIED,
            ffi::zvmc_storage_status::ZVMC_STORAGE_MODIFIED
        );
    }

    #[test]
    fn revision() {
        assert_eq!(Revision::ZVMC_SHANGHAI, ffi::zvmc_revision::ZVMC_SHANGHAI);
    }
}
