package aoc_2024_16_1

import "core:fmt"
import "core:log"
import "core:os"
import "core:strings"
import "core:strconv"
import "core:unicode/utf8"

import "core:testing"

Queue :: struct {
	items: [dynamic]int,
}

queue_init :: proc() -> Queue {
	return Queue{items = make([dynamic]int)}
}

queue_destroy :: proc(q: ^Queue) {
	delete(q.items)
}

enqueue :: proc(q: ^Queue, item: int) {
	append(&q.items, item)
}

dequeue :: proc(q: ^Queue) -> int {
	if len(q.items) > 0 {
		item := q.items[0]
		remove_range(&q.items, 0, 1)
		return item
	}
	log.panic("Tried to Deque the Queue when its empty")
}

queue_peek :: proc(q: Queue) -> (int, bool) {
	if len(q.items) == 0 {
		return 0, false
	}
	return q.items[0], true
}

queue_is_empty :: proc(q: Queue) -> bool {
	return len(q.items) == 0
}

custom_logger_proc :: proc(
	data: rawptr,
	level: log.Level,
	text: string,
	options: log.Options,
	location := #caller_location,
) {
	fmt.println(text)
}


pos :: struct {
	x: int,
	y: int,
}


main :: proc() {
	// logger := log.create_console_logger()

	logger := log.Logger {
		procedure    = custom_logger_proc,
		data         = nil,
		lowest_level = .Debug,
		options      = {},
	}

	context.logger = logger
	defer log.destroy_console_logger(logger)
	
	arguments := os.args[1:]
	
	if len(arguments) != 1 {
		log.panic("Usage: ./<program> <input_file> ")
	}
	
	// Open the file
	file_path := os.args[1]
	
	n_columns := 0
	n_rows := 0
	start_node: pos = {0,0} // always start at 0,0
	end_node: pos
	max_steps := 0
	if file_path == "short" {
		n_columns = 7
		n_rows = 7
		end_node.x = 6
		end_node.y = 6
		max_steps = 12
	}
	else if file_path == "input" {
		n_columns = 71
		n_rows = 71
		end_node.x = 70
		end_node.y = 70
		max_steps = 1024
	}


	data, success := os.read_entire_file(file_path)
	if !success {
		log.infof("Failed to open file '%s', exiting.\n", file_path)
		os.exit(1)
	}
	defer delete(data) // Free memory when we are done

	// Convert byte-data to string
	content := string(data)
	// Count row to determine grid array size
	lines := strings.split_lines(content)
	defer delete(lines)
	if len(lines) == 0 {
		log.panic("File is empty!")
	}

	// Read line by line to find next dropped blocks
	steps_taken := 0
	steps : [dynamic]pos
	for line in lines {
		if len(line) > 0 && steps_taken < max_steps{
			// Parse the line
			x := strconv.atoi(strings.split(line, ",")[0])
			y := strconv.atoi(strings.split(line, ",")[1])
			append(&steps, pos{x, y})
			steps_taken += 1
		}
	}

	
	// Create 2D-array (slice of slices)
	grid := make([][]rune, n_rows)
	defer {
		for row in grid {
			delete(row)
		}
		delete(grid)
	}
	
	// Fill the array with runes (UTF-8 tecken)
	for row in 0..<n_rows {
		grid[row] = make([]rune, n_columns)
		for col in 0..<n_columns {
			grid[row][col] = '.'
		}
	}
	
	// Print the array resulting from the file
	log.infof("Starting grid:")
	for row in grid {
		row_arr: [dynamic]rune
		defer delete(row_arr)
		for char in row {
			append(&row_arr, char)
		}
		log.infof("%s", row_arr)
	}
	

	for step in steps {
		grid[step.y][step.x] = '#'
	}

	log.infof("Blocked grid:")
	for row in grid {
		row_arr: [dynamic]rune
		defer delete(row_arr)
		for char in row {
			append(&row_arr, char)
		}
		log.infof("%s", row_arr)
	}

	solve_1(&grid, n_columns, n_rows, start_node, end_node)
	
}


solve_1 :: proc(grid: ^[][]rune, n_columns: int, n_rows: int, start_node: pos, end_node: pos) {
	
	R := n_rows // R = number of rows
	C := n_columns // C = number of columns
	
	// m := grid // input character matrix of size R * C
	
	sr := start_node.y // start node row number
	sc := start_node.x // start node column number
	
	rq := queue_init() // row queue
	defer queue_destroy(&rq)
	cq := queue_init() // column queue
	defer queue_destroy(&cq)

	// variables to track the number of steps taken
	moves_count := 0
	nodes_left_in_layer := 1
	nodes_in_next_layer := 0

	// varaible to track wether the E character ever gets reached during the BFS.
	reached_end := false

	// R * C matrix of false values used to track wether the node at position (i, j) has been visited.
	visited := make(map[pos]bool, R * C)
	defer delete(visited)

	// North, south, east, west direction vectors.
	horiz_dir_vec := [4]int{-1, 1, 0, 0}
	vert_dir_vec := [4]int{0, 0, 1, -1}

	enqueue(&rq, sr)
	enqueue(&cq, sc)
	visited[pos{sc, sr}] = true


	for !queue_is_empty(rq) && !queue_is_empty(cq) {
		r := dequeue(&rq)
		c := dequeue(&cq)


		if r == end_node.y && c == end_node.x {
			reached_end = true
			break
		}

		//explore neighbours to (r, c)
		for i in 0 ..< 4 {
			rr := r + horiz_dir_vec[i]
			cc := c + vert_dir_vec[i]

			// Skip out of bounds locations
			if rr < 0 || cc < 0 {continue}
			if rr >= R || cc >= C {continue}

			// Skip visited locations or blocked cells
			if visited[pos{cc, rr}] {continue}
			if grid[rr][cc] == '#' {continue}

			enqueue(&rq, rr)
			enqueue(&cq, cc)
			visited[pos{cc, rr}] = true
			nodes_in_next_layer += 1
		}


		nodes_left_in_layer -= 1
		if nodes_left_in_layer == 0 {
			nodes_left_in_layer = nodes_in_next_layer
			nodes_in_next_layer = 0
			moves_count += 1
		}
	}


	for row, row_i in grid {
		for col, col_i in row {
			if visited[pos{col_i, row_i}] {
				fmt.print("o")
			} else {
				fmt.print(col)
			}
		}
		fmt.print("\n")
	}
	if reached_end {
		log.infof("Shortest path found in %d steps", moves_count)
	} else {
		log.info("No path found to the end node")
	}
	// log.info("Sum of all boxes GPS coordinates is TBD")
}


solve_2 :: proc() {
	// TBD
}


@(test)
test_short :: proc(t: ^testing.T) {
	// test_grid : [][]rune = {
	// 	{'#', '#', '#', '#', '#', '#', '#', '#'},
	// 	{'#', '.', '.', '#', '.', '.', '.', '#'},
	// 	{'#', '.', '#', '@', '#', '.', '.', '#'},
	// 	{'#', '.', '.', '#', '.', '.', '.', '#'},
	// 	{'#', '#', '#', '#', '#', '#', '#', '#'}
	// }

	// solve_1(&test_grid, moves_arr, 8, 5, robot_posx_start, robot_posy_start)

	// testing.expect(t, expected_grid[row_i][char_i] == test_grid[row_i][char_i], "Expected res_grid to be the same as expected_grid after blocked move")
}
