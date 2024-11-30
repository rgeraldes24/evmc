package zvmc_use

import (
	"testing"

	"github.com/rgeraldes24/zvmc/v10/bindings/go/zvmc"
)

var exampleVmPath = "./example-vm.so"

func TestGetVmName(t *testing.T) {
	vm, err := zvmc.Load(exampleVmPath)
	if err != nil {
		t.Fatalf("%v", err)
	}

	expectedName := "example_vm"
	if name := vm.Name(); name != expectedName {
		t.Errorf("wrong VM name: %s, expected %s", name, expectedName)
	}
}
