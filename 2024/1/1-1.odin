package aoc_2024_1_1

import "core:fmt"
import "core:os"
import "core:slice"
import "core:strconv"
import "core:strings"


// variabel
// namn : typ = värde

// konstant
// namn : typ : värde

// typ är optional
// namn := värde
// namn :: värde

main :: proc() {

  	// öppna filen input
	// läs innehållet
	// skriv ut innehållet

	input_file_name :: "input"


	data, ok := os.read_entire_file(input_file_name, context.allocator)
	if !ok {
		fmt.println("Failed to read file: ", input_file_name)
		return
	}
	defer delete(data, context.allocator)

	left_values: [dynamic]int
	right_values: [dynamic]int

	it := string(data)
	for line in strings.split_lines_iterator(&it) {

		left := strings.split(line, " ")[0]
		right := strings.split(line, " ")[1]

		left_int := strconv.atoi(left)
		right_int := strconv.atoi(right)

		append(&left_values, left_int)
		append(&right_values, right_int)
	}

	// Sort arrays
	slice.sort(left_values[:])
	slice.sort(right_values[:])

  solve_1(left_values, right_values)
  solve_2(left_values, right_values)
}


solve_1 :: proc(left_values: [dynamic]int, right_values: [dynamic]int) {
	sum_distances := 0
	for cur, index in left_values {
		distance := cur - right_values[index]
    if distance < 0 {
      distance = -distance
    }
		sum_distances += distance
	}

	fmt.println("Sum_distances: ", sum_distances)

}


solve_2 :: proc(left_values: [dynamic]int, right_values: [dynamic]int) {

similarity_score := 0

for to_find in left_values{
  times_found := 0
  for right in right_values {
    if right == to_find {
      times_found += 1
    }
  }
  similarity_score += times_found * to_find
}

fmt.println("Similarity_score: ", similarity_score)
}