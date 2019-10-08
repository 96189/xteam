
package baseSearch

import (
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestBinSearch(t *testing.T) {
	useAssert := assert.New(t)
	s := []int{1,2,3,5,9}
	idx := BinSearch(s, 3)
	useAssert.Equal(2, idx, "")
	idx = BinSearch(s, 4)
	useAssert.Equal(-1, idx, "")
	idx = BinSearch(s, 1)
	useAssert.Equal(0, idx, "")
	idx = BinSearch(s, 9)
	useAssert.Equal(4, idx, "")
}