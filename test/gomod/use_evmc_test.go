package evmc_use

import (
	"testing"

	"github.com/rgeraldes24/evmc/v10/bindings/go/evmc"
)

var exampleVmPath = "./example-vm.so"

func TestGetVmName(t *testing.T) {
	vm, err := evmc.Load(exampleVmPath)
	if err != nil {
		t.Fatalf("%v", err)
	}

	expectedName := "example_vm"
	if name := vm.Name(); name != expectedName {
		t.Errorf("wrong VM name: %s, expected %s", name, expectedName)
	}
}
