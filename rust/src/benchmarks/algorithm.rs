use std::collections::HashMap;
use std::vec::Vec;

struct Board {
    size: usize,
    values: Vec::<bool>
}

#[derive(PartialEq, Eq, Hash, Clone)]
struct Point {
    i: usize,
    j: usize
}

impl Board {
    fn new(n: usize) -> Board {
        Board {
            size: n,
            values: vec![false; n*n]
        }
    }

    fn get(&self, p: &Point) -> bool

    {
        self.values[p.i*self.size + p.j]
    }

    fn set(&mut self,p: &Point, value: bool) -> ()
    {
        self.values[p.i*self.size + p.j] = value;
    }

    fn get_neighboors(&self, p: &Point) -> Vec::<Point>
    {
        let neighboors = vec![
            Point{i: p.i-1, j: p.j},
            Point{i: p.i+1, j: p.j},
            Point{i: p.i, j: p.j+1},
            Point{i: p.i, j: p.j-1},
        ];
        neighboors.into_iter().filter(|x| x.i < self.size && x.j < self.size).collect::<Vec<Point>>()
    }
}

fn walk(board: &Board, points: &Vec<Point>, previous: &mut HashMap<Point, bool>) -> Vec<Point>
{
    let mut next: Vec<Point> = Vec::new();
    next.reserve(4*previous.len());
    for p in points {
        let neighboors = board.get_neighboors(p);
        for n in neighboors {
            let insertion = previous.insert(n.clone(), true);
            if insertion.is_none() {
                next.push(n);
            }
        }
    }
    next
}

fn reached_end_game(board: &Board, points: &Vec<Point>) -> bool
{
    return points.iter().any(|x| board.get(&x));
}

pub fn benchmark_algorithm()
{
    const SIZE: usize = 1000;
    let mut board = Board::new(SIZE);
    board.set(&Point{i: 0, j: 0}, true);
    let mut points :Vec<Point> = vec![Point{i: SIZE-1, j: SIZE-1}];
    let mut previous = HashMap::<Point, bool>::new();
    let mut counter: u32 = 0;
    while points.len() > 0 && !reached_end_game(&board, &points) {
        points = walk(&board, &points, &mut previous);
        counter += 1;
    }
    assert!(counter == 2*(SIZE as u32-1));
}
