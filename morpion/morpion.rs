use std::io;

#[derive(Copy, Clone)]
struct Point {
    i: i32,
    j: i32
}

impl std::ops::Add<&Point> for Point {
    type Output = Point;
    fn add(mut self, rhs: &Point) -> Point {
        self += rhs;
        self
    }
}

impl std::ops::AddAssign<&Point> for Point {
    fn add_assign(&mut self, rhs: &Point) -> () {
        self.i += rhs.i;
        self.j += rhs.j;
    }
}

fn print_board(board: &[char]) -> ()
{
    println!("-------");
    for i in 0..3 {
        println!("|{}|{}|{}|", board[3*i], board[3*i+1], board[3*i+2]);
        println!("-------");
    }
}

fn is_winning_pos(
    board: &[char], 
    player: char, 
    start: &mut Point, 
    delta: &Point) -> Option<Point>
{
    let mut winning : Option<Point> = None;
    for _i in 0..3 {
        let val = board[(3*start.i + start.j) as usize];
        if val == ' ' {
            match winning {
                Some(_point) => return None,
                None         => winning = Some(*start),
            }
        }
        else if val != player {
            return None;
        }
        *start += delta;
    }
    return winning;
}

fn winning_pos(board: &[char], player: char) -> Option<usize>
{
    let rows: [(Point, Point); 8] = [
        (Point{i: 0, j: 0}, Point{i: 0, j: 1}),
        (Point{i: 0, j: 0}, Point{i: 1, j: 1}),
        (Point{i: 0, j: 0}, Point{i: 1, j: 0}),
        (Point{i: 0, j: 1}, Point{i: 1, j: 0}),
        (Point{i: 0, j: 2}, Point{i: 1, j: 0}),
        (Point{i: 1, j: 0}, Point{i: 0, j: 1}),
        (Point{i: 2, j: 0}, Point{i: 0, j: 1}),
        (Point{i: 2, j: 0}, Point{i: -1, j: 1})
    ];
    for (mut start, delta) in rows {
        let winning = is_winning_pos(board, player, &mut start, &delta);
        match winning {
            Some(point) => return Some((3*point.i + point.j) as usize),
            None => continue,
        }
    }
    None
}

fn first_empty(board: &[char]) -> usize
{
    for i in 0..9 {
        if board[i] == ' ' {
            return i;
        }
    }
    0
}

fn play_ia(board: &mut[char]) -> ()
{
    let win = winning_pos(&board, 'x');
    match win {
        Some(pos) => board[pos] = 'x',
        None => {
            let lose = winning_pos(&board, 'o');
            match lose {
                Some(pos) => board[pos] = 'x',
                None => board[first_empty(&board)] = 'x',
            }
        }
    }
}

fn play_human(board: &mut[char]) -> ()
{
    let mut pos: u32;
    loop {
        println!("Enter a position in [0, 9[: ");
        let mut input_text = String::new();
        io::stdin()
            .read_line(&mut input_text)
            .expect("Failed to read from stdin");

        let trimmed = input_text.trim();
        match trimmed.parse::<u32>() {
            Ok(i) => pos = i,
            Err(..) => {
                println!("This was not an integer: {}", trimmed);
                continue;
            }
        };
        if pos < 9 {
            break;
        }
    }
    board[pos as usize] = 'o';
}

fn end_game(board: &[char]) -> bool
{
    if board.iter().filter(|&n| *n == ' ').count() == 0 {
        return true;
    }
    return false;
}

fn game(board: &mut[char], human: bool) -> ()
{
    if end_game(board) {
        println!("The game has ended!");
        return;
    }
    if human {
        play_human(board);
    }
    else {
        play_ia(board);
    }
    print_board(board);
    game(board, !human);
}

fn main()
{
    let mut board: [char; 9] = [' '; 9];
    print_board(&board);
    game(&mut board, true);
}
