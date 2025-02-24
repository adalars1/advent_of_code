package aoc_2024_15_1

import "core:log"
import "core:os"
import "core:strings"
import "core:unicode/utf8"

import "core:testing"

// swap places of two runes in a 2D-array
swap_char :: proc(grid: ^[][]rune, x1: int, y1: int, x2: int, y2: int) {
	tmp := grid[y1][x1]
	grid[y1][x1] = grid[y2][x2]
	grid[y2][x2] = tmp
}


main :: proc() {
	logger := log.create_console_logger()
	context.logger = logger
	defer log.destroy_console_logger(logger)

	arguments := os.args[1:]

if len(arguments) != 1 {
	log.panic("Usage: ./<program> <input_file> ")
}

	// Open the file
	file_path := os.args[1]
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

	// Find length of the longest row
	max_width := 0
	max_height := 0
	for line in lines {
		if len(line) > 0 {
			if line[len(line) - 1] == '#' {
				width := utf8.rune_count_in_string(line)
				if width > max_width {
					max_width = width
				}
				max_height += 1
			}
		}	
	}
 

	// Create 2D-array (slice of slices)
	grid := make([][]rune, max_height)
	defer {
		for row in grid {
			delete(row)
		}
		delete(grid)
	}

	// Fill the array with runes (UTF-8 tecken)
	robot_posx_start := 0
	robot_posy_start := 0
	moves : [dynamic]rune
	for line, i in lines {
		if i < max_height{
			grid[i] = make([]rune, max_width)
			runes := utf8.string_to_runes(line)
			defer delete(runes)
			for r, j in runes {
				if j < max_width {
					if r == '#' || r == '.' || r == 'O' || r == '@' {
					grid[i][j] = r
					if r == '@' {
							robot_posx_start = j
							robot_posy_start = i
							log.infof("Found robot start at: %i %i\n", robot_posx_start, robot_posy_start)
						}
					}
				}
			}
		}
		else {
			runes := utf8.string_to_runes(line)
		defer delete(runes)
			for r, j in runes {
				if r == '<' || r == '>' || r == '^' || r == 'v' {
					append(&moves, r)
				}
			}
		}
	
	}

	// Print the array resulting from the file
	log.infof("Starting grid:")
	for row in grid {
		row_arr : [dynamic]rune
		defer delete(row_arr)
		for char in row {
			append(&row_arr, char)
		}
		log.infof("%s", row_arr)
	}

	log.info("moves after file parse: ", moves)
	solve_1(&grid, moves, max_width, max_height, robot_posx_start, robot_posy_start)

}




solve_1 :: proc(grid: ^[][]rune, moves: [dynamic]rune,
	              max_width: int, max_height: int,
								robot_posx_start: int, robot_posy_start: int) {

	robot_posx := robot_posx_start
	robot_posy := robot_posy_start
	for move in moves {
		left := robot_posx - 1
		right := robot_posx + 1
		up := robot_posy - 1
		down := robot_posy + 1

		log.infof("Move: %c", move)

		if move == '<' && robot_posx > 0 {
			if grid[robot_posy][left] == '.' {
				swap_char(grid, robot_posx, robot_posy, left, robot_posy)
			} 
			else if grid[robot_posy][left] == 'O' {
				// check if there is a space somewhere behind the O
				for leftmost_empty_idx := robot_posx;
				    leftmost_empty_idx > 0;
				    leftmost_empty_idx -= 1 {
					if grid[robot_posy][leftmost_empty_idx] == '#' {
						// we found a wall, abort search
						break
					}
					else if grid[robot_posy][leftmost_empty_idx] == '.' {
						// go back to robot position and move all barrels to the left
						for cur := leftmost_empty_idx; cur < robot_posx; cur += 1 {
							log.info("Swapping: cur: ", cur, " robot_pos_y: ", robot_posy, " with cur + 1: ", cur + 1, ", robot_pos_y: ", robot_posy)
							swap_char(grid, cur, robot_posy, cur + 1, robot_posy)
						}
						break
					}
				}
			}
		} 
		else if move == '>' {
			if grid[robot_posy][right] == '.' {
				swap_char(grid, robot_posx, robot_posy, right, robot_posy)
			}
			else if grid[robot_posy][right] == 'O' {
				// check if there is a space somewhere behind the O
				log.info("right: ", right, " robot_pos_x: ", robot_posx, " robot_pos_y: ", robot_posy)
				log.info("max_width: ", max_width, " max_height: ", max_height)
				for rightmost_empty_idx := right + 1;
				rightmost_empty_idx < max_width;
				rightmost_empty_idx += 1 {
					log.info("rightmost_empty_idx: ", rightmost_empty_idx)
					if grid[robot_posy][rightmost_empty_idx] == '#' {
						// we found a wall, abort search
						log.info("found wall at col: ", rightmost_empty_idx)
						break
					}
					else if grid[robot_posy][rightmost_empty_idx] == '.' {
						// go back to robot position and move all barrels to the right
						log.info("found a space at col: ", rightmost_empty_idx)
						for cur := rightmost_empty_idx; cur > robot_posx; cur -= 1 {
							log.info("Swapping: cur: ", cur, " robot_pos_y: ", robot_posy, " with cur - 1: ", cur - 1, ", robot_pos_y: ", robot_posy)
							swap_char(grid, cur, robot_posy, cur - 1, robot_posy)
						}
						break
					}
				}
			}
		}
		 else if move == '^' {
			if grid[up][robot_posx] == '.' {
				swap_char(grid, robot_posx, robot_posy, robot_posx, up)
			}
			else if grid[up][robot_posx] == 'O' {
				// check if there is a space somewhere behind the O
				for upper_empty_idx := up - 1;
				upper_empty_idx > 0;
				upper_empty_idx -= 1 {
					if grid[upper_empty_idx][robot_posx] == '#' {
						// we found a wall, abort search
						break
					}
					else if grid[upper_empty_idx][robot_posx] == '.' {
						// go back to robot position and move all barrels upwards
						for cur := upper_empty_idx; cur < robot_posy; cur += 1 {
							log.info("Swapping: robot_pos_x: ", robot_posx, " cur: ", cur, " with robot_pos_x: ", robot_posx, ", cur + 1: ", cur + 1)
							swap_char(grid, robot_posx, cur, robot_posx, cur + 1)
						}
						break
					}
				}
			}
		} 
		else if move == 'v' {
			if grid[down][robot_posx] == '.' {
				swap_char(grid, robot_posx, robot_posy, robot_posx, down)
			}
			else if grid[down][robot_posx] == 'O' {
				// check if there is a space somewhere behind the O
				for lower_empty_idx := down + 1;
				lower_empty_idx < max_height;
				lower_empty_idx += 1 {
					if grid[lower_empty_idx][robot_posx] == '#' {
						// we found a wall, abort search
						break
					}
					else if grid[lower_empty_idx][robot_posx] == '.' {
						// go back to robot position and move all barrels downwards
						for cur := lower_empty_idx; cur > robot_posy; cur -= 1 {
							log.info("Swapping: robot_pos_x: ", robot_posx, " cur: ", cur, " with robot_pos_x: ", robot_posx, ", cur - 1: ", cur - 1)
							swap_char(grid, robot_posx, cur, robot_posx, cur - 1)
						}
						break
					}
				}
			}
		}
		
		for row, row_ix in grid {
			row_arr : [dynamic]rune
			defer delete(row_arr)
			for char, char_ix in row {
				append(&row_arr, char)
				if char == '@' {
					// update robot pos after move
					robot_posx = char_ix
					robot_posy = row_ix
				}
			}
			log.infof("%s", row_arr)
		}
		
	}

}


solve_2 :: proc() {
	// TBD
}



@(test)
test_blocked :: proc(t: ^testing.T) {
  
	moves := "<<<<<<^^^^^^vvvvvvvvvv>>>>>>><<<<>>><"
	moves_arr : [dynamic]rune
	defer delete(moves_arr)

	for r in moves {
		append(&moves_arr, r)
	}

	test_grid : [][]rune = {
		{'#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '.', '.', '#', '.', '.', '.', '#'},
		{'#', '.', '#', '@', '#', '.', '.', '#'},
		{'#', '.', '.', '#', '.', '.', '.', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#'}
	}

	robot_posx_start := 2
	robot_posy_start := 3

	expected_grid := test_grid

	solve_1(&test_grid, moves_arr, 8, 5, robot_posx_start, robot_posy_start)

	testing.expect(t, test_grid[2][3] == '@', "Expected robot to be at position 2, 3")
	for row, row_i in test_grid {
		for char, char_i in row {
			ok := testing.expect(t, expected_grid[row_i][char_i] == test_grid[row_i][char_i], "Expected res_grid to be the same as expected_grid after blocked move")
			if !ok {
				log.infof("Expected: %c, got: %c\n", expected_grid[row_i][char_i], test_grid[row_i][char_i])
			}
		}
	}
}


@(test)
test_shorter :: proc(t: ^testing.T) {
  
	moves := "<^^>>>vv<v>>v<<"
	moves_arr : [dynamic]rune
	defer delete(moves_arr)

	for r in moves {
		append(&moves_arr, r)
	}
	test_grid : [][]rune = {
		{'#', '#', '#', '#', '#', '#', '#', '#'}, 
		{'#', '.', '.', 'O', '.', 'O', '.', '#'}, 
		{'#', '#', '@', '.', 'O', '.', '.', '#'}, 
		{'#', '.', '.', '.', 'O', '.', '.', '#'}, 
		{'#', '.', '#', '.', 'O', '.', '.', '#'}, 
		{'#', '.', '.', '.', 'O', '.', '.', '#'}, 
		{'#', '.', '.', '.', '.', '.', '.', '#'}, 
		{'#', '#', '#', '#', '#', '#', '#', '#'}, 
	}
	robot_posx_start := 2
	robot_posy_start := 2

	expected_grid : [][]rune = {
		{'#', '#', '#', '#', '#', '#', '#', '#'}, 
		{'#', '.', '.', '.', '.', 'O', 'O', '#'}, 
		{'#', '#', '.', '.', '.', '.', '.', '#'}, 
		{'#', '.', '.', '.', '.', '.', 'O', '#'}, 
		{'#', '.', '#', 'O', '@', '.', '.', '#'}, 
		{'#', '.', '.', '.', 'O', '.', '.', '#'}, 
		{'#', '.', '.', '.', 'O', '.', '.', '#'}, 
		{'#', '#', '#', '#', '#', '#', '#', '#'}, 
	}

	max_width := len(test_grid[0])
	max_height := len(test_grid)

	solve_1(&test_grid, moves_arr, max_width, max_height, robot_posx_start, robot_posy_start)

	testing.expect(t, test_grid[4][4] == '@', "Expected robot to be at position 4, 4")
	for row, row_i in test_grid {
		for char, char_i in row {
			testing.expectf(t, expected_grid[row_i][char_i] == test_grid[row_i][char_i],
				              "At [%i][%i]: Expected: %c, got: %c\n",
											 row_i, char_i, expected_grid[row_i][char_i], test_grid[row_i][char_i])
		}
	}
}



@(test)
test_longer :: proc(t: ^testing.T) {
  
	moves := "<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^"
	moves_arr : [dynamic]rune
	defer delete(moves_arr)

	for r in moves {
		append(&moves_arr, r)
	}

	test_grid : [][]rune = {
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}, 
		{'#', '.', '.', 'O', '.', '.', 'O', '.', 'O', '#'}, 
		{'#', '.', '.', '.', '.', '.', '.', 'O', '.', '#'}, 
		{'#', '.', 'O', 'O', '.', '.', 'O', '.', 'O', '#'}, 
		{'#', '.', '.', 'O', '@', '.', '.', 'O', '.', '#'}, 
		{'#', 'O', '#', '.', '.', 'O', '.', '.', '.', '#'}, 
		{'#', 'O', '.', '.', 'O', '.', '.', 'O', '.', '#'}, 
		{'#', '.', 'O', 'O', '.', 'O', '.', 'O', 'O', '#'}, 
		{'#', '.', '.', '.', '.', 'O', '.', '.', '.', '#'}, 
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	}
	robot_posx_start := 4
	robot_posy_start := 4

	expected_grid : [][]rune = {
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}, 
		{'#', '.', 'O', '.', 'O', '.', 'O', 'O', 'O', '#'}, 
		{'#', '.', '.', '.', '.', '.', '.', '.', '.', '#'}, 
		{'#', 'O', 'O', '.', '.', '.', '.', '.', '.', '#'}, 
		{'#', 'O', 'O', '@', '.', '.', '.', '.', '.', '#'}, 
		{'#', 'O', '#', '.', '.', '.', '.', '.', 'O', '#'}, 
		{'#', 'O', '.', '.', '.', '.', '.', 'O', 'O', '#'}, 
		{'#', 'O', '.', '.', '.', '.', '.', 'O', 'O', '#'}, 
		{'#', 'O', 'O', '.', '.', '.', '.', 'O', 'O', '#'}, 
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}, 
	}

	max_width := len(test_grid[0])
	max_height := len(test_grid)

	solve_1(&test_grid, moves_arr, max_width, max_height, robot_posx_start, robot_posy_start)

	testing.expect(t, test_grid[4][3] == '@', "Expected robot to be at row 4, col 3")
	for row, row_i in test_grid {
		for char, char_i in row {
			testing.expectf(t, expected_grid[row_i][char_i] == test_grid[row_i][char_i],
				              "At [%i][%i]: Expected: %c, got: %c\n",
											 row_i, char_i, expected_grid[row_i][char_i], test_grid[row_i][char_i])
		}
	}
}