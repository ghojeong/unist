from sol5 import *

#  CSE341 Spring 2016, Homework 5, Provided Tests
#  Will not work completely until you implement all the classes and their methods
#
#  Will print only if code has errors; prints nothing if all tests pass
#
#  These tests do NOT cover all the various cases, especially for intersection

#Constants for testing
ZERO = 0.0
ONE = 1.0
TWO = 2.0
THREE = 3.0
FOUR = 4.0
FIVE = 5.0
SIX = 6.0
SEVEN = 7.0
TEN = 10.0

#Point Tests
a = Point(THREE,FIVE)
if not (a.x == THREE and a.y == FIVE):
    print("Point is not initialized properly")

if not (a.eval_prog({}) == a):
    print("Point eval_prog should return self")

if not (a.preprocess_prog() == a):
    print("Point preprocess_prog should return self")

a1 = a.shift(THREE,FIVE)
if not (a1.x == SIX and a1.y == TEN):
    print("Point shift not working properly")

a2 = a.intersect(Point(THREE,FIVE))
if not (a2.x == THREE and a2.y == FIVE):
    print("Point intersect not working properly")

a3 = a.intersect(Point(FOUR,FIVE))
if not isinstance(a3, NoPoints):
    print("Point intersect not working properly")

#Line Tests
b = Line(THREE,FIVE)
if not (b.m == THREE and b.b == FIVE):
    print("Line not initialized properly")

if not (b.eval_prog({}) == b):
    print("Line eval_prog should return self")

if not (b.preprocess_prog() == b):
    print("Line preprocess_prog should return self")

b1 = b.shift(THREE,FIVE) 
if not (b1.m == THREE and b1.b == ONE):
    print("Line shift not working properly")

b2 = b.intersect(Line(THREE,FIVE))
if not (isinstance(b2, Line) and b2.m == THREE and b2.b == FIVE):
    print("Line intersect not working properly")

b3 = b.intersect(Line(THREE,FOUR))
if not isinstance(b3, NoPoints):
    print("Line intersect not working properly")

#VerticalLine Tests
c = VerticalLine(THREE)
if not (c.x == THREE):
    print("VerticalLine not initialized properly")

if not (c.eval_prog({}) == c):
    print("VerticalLine eval_prog should return self")

if not (c.preprocess_prog() == c):
    print("VerticalLine preprocess_prog should return self")

c1 = c.shift(THREE,FIVE)
if not (c1.x == SIX):
    print("VerticalLine shift not working properly")

c2 = c.intersect(VerticalLine(THREE))
if not (isinstance(c2, VerticalLine) and c2.x == THREE ):
    print("VerticalLine intersect not working properly")

c3 = c.intersect(VerticalLine(FOUR))
if not (isinstance(c3, NoPoints)):
    print("VerticalLine intersect not working properly")

#LineSegment Tests
d = LineSegment(ONE,TWO,-THREE,-FOUR)
if not (d.eval_prog({}) == d):
    print("LineSegement eval_prog should return self")

d1 = LineSegment(ONE,TWO,ONE,TWO)
d2 = d1.preprocess_prog()
if not (isinstance(d2, Point) and d2.x == ONE and d2.y == TWO) :
    print("LineSegment preprocess_prog should convert to a Point")
    print("if ends of segment are real_close")

d = d.preprocess_prog()
if not (d.x1 == -THREE and d.y1 == -FOUR and d.x2 == ONE and d.y2 == TWO):
    print("LineSegment preprocess_prog should make x1 and y1")
    print("on the left of x2 and y2")

d3 = d.shift(THREE,FIVE)
if not (d3.x1 == ZERO and d3.y1 == ONE and d3.x2 == FOUR and d3.y2 == SEVEN):
    print("LineSegment shift not working properly")

d4 = d.intersect(LineSegment(-THREE,-FOUR,ONE,TWO))
if not (isinstance(d4, LineSegment) and d4.x1 == -THREE and d4.y1 == -FOUR and d4.x2 == ONE and d4.y2 == TWO):
    print("LineSegment intersect not working properly")

d5 = d.intersect(LineSegment(TWO,THREE,FOUR,FIVE))
if not isinstance(d5, NoPoints):
    print("LineSegment intersect not working properly")

#Intersect Tests
i = Intersect(LineSegment(-ONE,-TWO,THREE,FOUR), LineSegment(THREE,FOUR,-ONE,-TWO))
i1 = i.preprocess_prog().eval_prog({})
if not (i1.x1 == -ONE and i1.y1 == -TWO and i1.x2 == THREE and i1.y2 == FOUR):
    print("Intersect eval_prog should return the intersect between e1 and e2")

#Var Tests
v = Var("a")
v1 = v.eval_prog({"a": Point(THREE,FIVE)})
if not (isinstance(v1, Point) and v1.x == THREE and v1.y == FIVE):
    print("Var eval_prog is not working properly")

if not (v1.preprocess_prog() == v1):
    print("Var preprocess_prog should return self")

#Let Tests
l = Let("a", LineSegment(-ONE,-TWO,THREE,FOUR),
             Intersect(Var("a"),LineSegment(THREE,FOUR,-ONE,-TWO)))
l1 = l.preprocess_prog().eval_prog({})
if not (l1.x1 == -ONE and l1.y1 == -TWO and l1.x2 == THREE and l1.y2 == FOUR):
    print("Let eval_prog should evaluate e2 after adding [s, e1] to the environment")

#Let Variable Shadowing Test
l2 = Let("a", LineSegment(-ONE, -TWO, THREE, FOUR),
              Let("b", LineSegment(THREE,FOUR,-ONE,-TWO), Intersect(Var("a"),Var("b"))))

l2 = l2.preprocess_prog().eval_prog({"a":Point(0,0)})
if not (l2.x1 == -ONE and l2.y1 == -TWO and l2.x2 == THREE and l2.y2 == FOUR):
    print("Let eval_prog should evaluate e2 after adding [s, e1] to the environment")

#Shift Tests
s = Shift(THREE,FIVE,LineSegment(-ONE,-TWO,THREE,FOUR))
s1 = s.preprocess_prog().eval_prog({})
if not (s1.x1 == TWO and s1.y1 == THREE and s1.x2 == SIX and s1.y2 == 9):
    print("Shift should shift e by dx and dy")
