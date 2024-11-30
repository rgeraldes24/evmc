// EVMC: Ethereum Client-VM Connector API.
// Copyright 2018 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

package zvmc

/*
#cgo CFLAGS: -I${SRCDIR}/../../../include -Wall -Wextra -Wno-unused-parameter

#include <zvmc/zvmc.h>
#include <zvmc/helpers.h>

*/
import "C"
import (
	"unsafe"
)

type CallKind int

const (
	Call         CallKind = C.ZVMC_CALL
	DelegateCall CallKind = C.ZVMC_DELEGATECALL
	Create       CallKind = C.ZVMC_CREATE
	Create2      CallKind = C.ZVMC_CREATE2
)

type AccessStatus int

const (
	ColdAccess AccessStatus = C.ZVMC_ACCESS_COLD
	WarmAccess AccessStatus = C.ZVMC_ACCESS_WARM
)

type StorageStatus int

const (
	StorageAssigned         StorageStatus = C.ZVMC_STORAGE_ASSIGNED
	StorageAdded            StorageStatus = C.ZVMC_STORAGE_ADDED
	StorageDeleted          StorageStatus = C.ZVMC_STORAGE_DELETED
	StorageModified         StorageStatus = C.ZVMC_STORAGE_MODIFIED
	StorageDeletedAdded     StorageStatus = C.ZVMC_STORAGE_DELETED_ADDED
	StorageModifiedDeleted  StorageStatus = C.ZVMC_STORAGE_MODIFIED_DELETED
	StorageDeletedRestored  StorageStatus = C.ZVMC_STORAGE_DELETED_RESTORED
	StorageAddedDeleted     StorageStatus = C.ZVMC_STORAGE_ADDED_DELETED
	StorageModifiedRestored StorageStatus = C.ZVMC_STORAGE_MODIFIED_RESTORED
)

func goAddress(in C.zvmc_address) Address {
	out := Address{}
	for i := 0; i < len(out); i++ {
		out[i] = byte(in.bytes[i])
	}
	return out
}

func goHash(in C.zvmc_bytes32) Hash {
	out := Hash{}
	for i := 0; i < len(out); i++ {
		out[i] = byte(in.bytes[i])
	}
	return out
}

func goByteSlice(data *C.uint8_t, size C.size_t) []byte {
	if size == 0 {
		return []byte{}
	}
	return (*[1 << 30]byte)(unsafe.Pointer(data))[:size:size]
}

// TxContext contains information about current transaction and block.
type TxContext struct {
	GasPrice   Hash
	Origin     Address
	Coinbase   Address
	Number     int64
	Timestamp  int64
	GasLimit   int64
	PrevRandao Hash
	ChainID    Hash
	BaseFee    Hash
}

type HostContext interface {
	AccountExists(addr Address) bool
	GetStorage(addr Address, key Hash) Hash
	SetStorage(addr Address, key Hash, value Hash) StorageStatus
	GetBalance(addr Address) Hash
	GetCodeSize(addr Address) int
	GetCodeHash(addr Address) Hash
	GetCode(addr Address) []byte
	GetTxContext() TxContext
	GetBlockHash(number int64) Hash
	EmitLog(addr Address, topics []Hash, data []byte)
	Call(kind CallKind,
		recipient Address, sender Address, value Hash, input []byte, gas int64, depth int,
		static bool, salt Hash, codeAddress Address) (output []byte, gasLeft int64, gasRefund int64,
		createAddr Address, err error)
	AccessAccount(addr Address) AccessStatus
	AccessStorage(addr Address, key Hash) AccessStatus
}

//export accountExists
func accountExists(pCtx unsafe.Pointer, pAddr *C.zvmc_address) C.bool {
	ctx := getHostContext(uintptr(pCtx))
	return C.bool(ctx.AccountExists(goAddress(*pAddr)))
}

//export getStorage
func getStorage(pCtx unsafe.Pointer, pAddr *C.struct_zvmc_address, pKey *C.zvmc_bytes32) C.zvmc_bytes32 {
	ctx := getHostContext(uintptr(pCtx))
	return zvmcBytes32(ctx.GetStorage(goAddress(*pAddr), goHash(*pKey)))
}

//export setStorage
func setStorage(pCtx unsafe.Pointer, pAddr *C.zvmc_address, pKey *C.zvmc_bytes32, pVal *C.zvmc_bytes32) C.enum_zvmc_storage_status {
	ctx := getHostContext(uintptr(pCtx))
	return C.enum_zvmc_storage_status(ctx.SetStorage(goAddress(*pAddr), goHash(*pKey), goHash(*pVal)))
}

//export getBalance
func getBalance(pCtx unsafe.Pointer, pAddr *C.zvmc_address) C.zvmc_uint256be {
	ctx := getHostContext(uintptr(pCtx))
	return zvmcBytes32(ctx.GetBalance(goAddress(*pAddr)))
}

//export getCodeSize
func getCodeSize(pCtx unsafe.Pointer, pAddr *C.zvmc_address) C.size_t {
	ctx := getHostContext(uintptr(pCtx))
	return C.size_t(ctx.GetCodeSize(goAddress(*pAddr)))
}

//export getCodeHash
func getCodeHash(pCtx unsafe.Pointer, pAddr *C.zvmc_address) C.zvmc_bytes32 {
	ctx := getHostContext(uintptr(pCtx))
	return zvmcBytes32(ctx.GetCodeHash(goAddress(*pAddr)))
}

//export copyCode
func copyCode(pCtx unsafe.Pointer, pAddr *C.zvmc_address, offset C.size_t, p *C.uint8_t, size C.size_t) C.size_t {
	ctx := getHostContext(uintptr(pCtx))
	code := ctx.GetCode(goAddress(*pAddr))
	length := C.size_t(len(code))

	if offset >= length {
		return 0
	}

	toCopy := length - offset
	if toCopy > size {
		toCopy = size
	}

	out := goByteSlice(p, size)
	copy(out, code[offset:])
	return toCopy
}

//export getTxContext
func getTxContext(pCtx unsafe.Pointer) C.struct_zvmc_tx_context {
	ctx := getHostContext(uintptr(pCtx))

	txContext := ctx.GetTxContext()

	return C.struct_zvmc_tx_context{
		zvmcBytes32(txContext.GasPrice),
		zvmcAddress(txContext.Origin),
		zvmcAddress(txContext.Coinbase),
		C.int64_t(txContext.Number),
		C.int64_t(txContext.Timestamp),
		C.int64_t(txContext.GasLimit),
		zvmcBytes32(txContext.PrevRandao),
		zvmcBytes32(txContext.ChainID),
		zvmcBytes32(txContext.BaseFee),
	}
}

//export getBlockHash
func getBlockHash(pCtx unsafe.Pointer, number int64) C.zvmc_bytes32 {
	ctx := getHostContext(uintptr(pCtx))
	return zvmcBytes32(ctx.GetBlockHash(number))
}

//export emitLog
func emitLog(pCtx unsafe.Pointer, pAddr *C.zvmc_address, pData unsafe.Pointer, dataSize C.size_t, pTopics unsafe.Pointer, topicsCount C.size_t) {
	ctx := getHostContext(uintptr(pCtx))

	// FIXME: Optimize memory copy
	data := C.GoBytes(pData, C.int(dataSize))
	tData := C.GoBytes(pTopics, C.int(topicsCount*32))

	nTopics := int(topicsCount)
	topics := make([]Hash, nTopics)
	for i := 0; i < nTopics; i++ {
		copy(topics[i][:], tData[i*32:(i+1)*32])
	}

	ctx.EmitLog(goAddress(*pAddr), topics, data)
}

//export call
func call(pCtx unsafe.Pointer, msg *C.struct_zvmc_message) C.struct_zvmc_result {
	ctx := getHostContext(uintptr(pCtx))

	kind := CallKind(msg.kind)
	output, gasLeft, gasRefund, createAddr, err := ctx.Call(kind, goAddress(msg.recipient), goAddress(msg.sender), goHash(msg.value),
		goByteSlice(msg.input_data, msg.input_size), int64(msg.gas), int(msg.depth), msg.flags != 0, goHash(msg.create2_salt),
		goAddress(msg.code_address))

	statusCode := C.enum_zvmc_status_code(0)
	if err != nil {
		statusCode = C.enum_zvmc_status_code(err.(Error))
	}

	outputData := (*C.uint8_t)(nil)
	if len(output) > 0 {
		outputData = (*C.uint8_t)(&output[0])
	}

	result := C.zvmc_make_result(statusCode, C.int64_t(gasLeft), C.int64_t(gasRefund), outputData, C.size_t(len(output)))
	result.create_address = zvmcAddress(createAddr)
	return result
}

//export accessAccount
func accessAccount(pCtx unsafe.Pointer, pAddr *C.zvmc_address) C.enum_zvmc_access_status {
	ctx := getHostContext(uintptr(pCtx))
	return C.enum_zvmc_access_status(ctx.AccessAccount(goAddress(*pAddr)))
}

//export accessStorage
func accessStorage(pCtx unsafe.Pointer, pAddr *C.zvmc_address, pKey *C.zvmc_bytes32) C.enum_zvmc_access_status {
	ctx := getHostContext(uintptr(pCtx))
	return C.enum_zvmc_access_status(ctx.AccessStorage(goAddress(*pAddr), goHash(*pKey)))
}
