package baseSort
import (
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestBubbleSort(t *testing.T) {
	useAssert := assert.New(t)
	s := [3][]int{
		{1,1,2,2,3,3,4,4,5,5},
		{5,5,4,4,3,3,2,2,1,1},
		{1,2,3,4,5,1,2,3,4,5},		
	}
	d := []int{1,1,2,2,3,3,4,4,5,5}
	for _, seq := range s {
		bubbleSort(seq[:])
		useAssert.Equal(d, seq, "sortSuccess")
	}
}