package aoc_2024_13_1

import "core:log"
import "core:os"
import "core:strings"
import "core:strconv"
import "core:unicode/utf8"

import "core:testing"

button :: struct {
	x : int,
	y : int
}

game :: struct {
	button_a : button,
	button_b : button,
	price_x : int,
	price_y : int,
	lowest_cost : int
}

INFINITE_COST :: 99999

main :: proc() {

	games : [dynamic]game
	defer delete(games)

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
 
	tmp_game : game
	append(&games, tmp_game)
	game_idx := 0
	for line, i in lines {
		log.info(line)
		split_line := strings.split(line, " ")
			if len(split_line) > 3 {
				btn_type := strings.split(split_line[1], ":")[0]

				x_btn := split_line[2]
				x_btn_val := strconv.atoi(strings.split(x_btn, "+")[1])
				
				y_btn := split_line[3]
				y_btn_val := strconv.atoi(strings.split(y_btn, "+")[1])

				if btn_type == "A" {
				games[game_idx].button_a.x = x_btn_val
				games[game_idx].button_a.y = y_btn_val

				}
				else if btn_type == "B" {
					games[game_idx].button_b.x = x_btn_val
					games[game_idx].button_b.y = y_btn_val
					
				}
			}
			else if len(split_line) == 3 {
				price_x_pos_val := strconv.atoi(strings.split(split_line[1], "=")[1])
				price_y_pos_val := strconv.atoi(strings.split(split_line[2], "=")[1])

				games[game_idx].price_x = price_x_pos_val
				games[game_idx].price_y = price_y_pos_val
				games[game_idx].lowest_cost = INFINITE_COST

				// prepare a new game for next iteration
				game_idx += 1
				next_game : game
				append(&games, next_game)
			}
			
		}	
	pop(&games) // delete nonexistent next game
	
	solve_1(&games)
}


solve_1 :: proc(games : ^[dynamic]game) {
	for &g, i in games {
		log.info("---------- Game", i, "------------")
		log.info("Button A: X+", g.button_a.x, "Y+", g.button_a.y)
		log.info("Button B: X+", g.button_b.x, "Y+", g.button_b.y)
		log.info("Price: X=", g.price_x, "Y=", g.price_y)

		// each button can be pressed at most 100 times
		// walk through all combinations of button presses
		for a_presses in 0..=100 {
			for b_presses in 0..=100 {
				a_end_pos_x := g.button_a.x * a_presses
				a_end_pos_y := g.button_a.y * a_presses
				
				b_end_pos_x := g.button_b.x * b_presses
				b_end_pos_y := g.button_b.y * b_presses
				
				end_pos_x := a_end_pos_x + b_end_pos_x
				end_pos_y := a_end_pos_y + b_end_pos_y

				if end_pos_x == g.price_x && end_pos_y == g.price_y {
					// button press combination lands on price position
					log.info("we hit the price position at", a_presses, " A presses, ", b_presses, " B presses.")
					combination_cost := a_presses * 3 + b_presses
					if combination_cost < g.lowest_cost {
						// cheaper combination found
						g.lowest_cost = combination_cost
					}
				}
			}
		}
	}
	
	sum_tokens := 0
	for g in games {
		if g.lowest_cost < INFINITE_COST {
			sum_tokens += g.lowest_cost
		}
	}
	log.info("Sum of fewest tokens to spend: ", sum_tokens)
}


solve_2 :: proc() {
	// TBD
}



@(test)
test_blocked :: proc(t: ^testing.T) {
  
	// solve_1()

	// testing.expect(t, test_grid[2][3] == '@', "Expected robot to be at position 2, 3")
}
