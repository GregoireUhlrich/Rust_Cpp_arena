use std::rc::Rc;
use std::any::Any;


pub trait ExprToAny: 'static {
    fn as_any(&self) -> &dyn Any;
}

impl<T: 'static> ExprToAny for T {
    fn as_any(&self) -> &dyn Any {
        self
    }
}

pub trait BaseExpr: ExprToAny {

    fn to_string(&self) -> String;

    fn print(&self) -> () {
        println!("{}", self.to_string());
    }

    fn replace(&self, _old: &Expr, _new: &Expr) -> Option<Expr> {
        None
    }

    fn is_equal(&self, other: &Expr) -> bool;

    fn as_number(&self) -> Option<&Number> {
        None
    }

    fn as_symbol(&self) -> Option<&Symbol> {
        None
    }

    fn as_sum(&self) -> Option<&Sum> {
        None
    }
}

pub type Expr = Rc<dyn BaseExpr>;

pub struct Number {
    pub val: i32
}

pub struct Symbol {
    pub name: &'static str
}

pub struct Sum {
    pub args: Vec<Expr>
}

impl BaseExpr for Number {

    fn to_string(&self) -> String {
        self.val.to_string()
    }

    fn is_equal(&self, other: &Expr) -> bool {
        let converted = other.as_number();
        return match converted {
            Some(x)    => (self.val == x.val),
            _default => false
        }
    }

    fn as_number(&self) -> Option<&Number> {
        return Some(self);
    }
}

impl BaseExpr for Symbol {

    fn to_string(&self) -> String {
        self.name.to_string()
    }

    fn is_equal(&self, other: &Expr) -> bool {
        let converted = other.as_symbol();
        return match converted {
            Some(x)    => (self.name == x.name),
            _default => false
        }
    }

    fn as_symbol(&self) -> Option<&Symbol> {
        return Some(self);
    }

    fn replace(&self, old: &Expr, new: &Expr) -> Option<Expr> {
        if self.is_equal(old) {
            Some(new.clone())
        }
        else {
            None
        }
    }
}

impl BaseExpr for Sum {

    fn to_string(&self) -> String {
        self.args.iter().map(|x| x.to_string()).collect::<Vec<String>>().join(" + ")
    }

    fn is_equal(&self, other: &Expr) -> bool {
        let converted = other.as_sum();
        return match converted {
            Some(x)    => {
                if x.args.len() != x.args.len() {
                    false
                }
                else {
                    for i in 0..self.args.len() {
                        if !self.args[i].is_equal(&x.args[i]) {
                            return false;
                        }
                    }
                    true
                }
            },
            _default => false
        }
    }

    fn as_sum(&self) -> Option<&Sum> {
        return Some(self);
    }

    fn replace(&self, old: &Expr, new: &Expr) -> Option<Expr> {
        if self.is_equal(old) {
            Some(new.clone())
        }
        else {
            let mut identical: bool = true;
            let identical_ptr = &mut identical;
            let new_args = self.args.iter().map(
                |x| match x.replace(old, new) {
                Some(expr) => {
                    *identical_ptr = false;
                    expr
                }
                None => x.clone()

            }).collect();
            if identical {
                None
            }
            else {
                Some(sum(new_args))
            }
        }
    }
}

pub fn number(val: i32) -> Expr
{
    Rc::new(Number{ val })
}

pub fn symbol(name: &'static str) -> Expr
{
    Rc::new(Symbol { name })
}

pub fn sum(args: Vec<Expr>) -> Expr
{
    Rc::new(Sum{ args })
}

pub fn replace(expr: &Expr, old: &Expr, new: &Expr) -> Expr
{
    match expr.replace(old, new) {
        Some(replaced) => replaced,
        None => expr.clone()
    }
}

pub fn benchmark_algebra() {

    let a = number(3);
    let b = number(4);
    let x = symbol("x");
    let y = symbol("y");
    let mut s1 = sum(Vec::from([a.clone(), x.clone()]));
    let s2 = sum(Vec::from([b.clone(), y.clone()]));
    s1 = replace(&s1, &a, &b);
    s1 = replace(&s1, &x, &y);
    assert!(!a.is_equal(&b));
    assert!(!x.is_equal(&y));
    assert!(!s1.is_equal(&s2));

    let big_sum1 = sum(vec![x.clone(); 1000]);
    let mut big_sum2 = sum(vec![big_sum1.clone(); 1000]);
    let big_sum3 = sum(vec![a.clone(); 1000]);
    let big_sum4 = sum(vec![big_sum3.clone(); 1000]);
    big_sum2 = replace(&replace(&big_sum2, &x, &y), &y, &a);
    assert!(big_sum2.is_equal(&big_sum4));
}
