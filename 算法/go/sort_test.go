package baseSort
import (
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestSelectSort(t *testing.T) {
	useAssert := assert.New(t)
	s := [][]int{
		{5,8,5,2,9},
		{5,5,8,2,9},
		{2,5,5,8,9},	
		{9,8,5,5,2},	
	}
	d := []int{2,5,5,8,9}
	for _, seq := range s {
		SelectSort(seq)
		useAssert.Equal(d, seq, "sortSuccess")
	}
}

func TestBubbleSort(t *testing.T) {
	useAssert := assert.New(t)
	s := [][]int{
		{5,8,5,2,9},
		{5,5,8,2,9},
		{2,5,5,8,9},	
		{9,8,5,5,2},	
	}
	d := []int{2,5,5,8,9}
	for _, seq := range s {
		BubbleSort(seq)
		useAssert.Equal(d, seq, "sortSuccess")
	}
}

func TestInsertSort(t *testing.T) {
	useAssert := assert.New(t)
	s := [][]int{
		{5,8,5,2,9},
		{5,5,8,2,9},
		{2,5,5,8,9},	
		{9,8,5,5,2},	
	}
	d := []int{2,5,5,8,9}
	for _, seq := range s {
		InsertSort(seq)
		useAssert.Equal(d, seq, "sortSuccess")
	}
}