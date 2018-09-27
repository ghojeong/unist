# 20131674 Gho JeongWan
# CSE341 Spring 2016, Homework 5, hw5.py (see also SML code)

# a little language for 2D geometry objects

# each subclass of GeometryExpression, including subclasses of GeometryValue,
#  needs to respond to messages preprocess_prog and eval_prog
#
# each subclass of GeometryValue additionally needs:
#   * shift
#   * intersect, which uses the double-dispatch pattern
#   * intersectPoint, intersectLine, and intersectVerticalLine for 
#       for being called by intersect of appropriate clases and doing
#       the correct intersection calculuation
#   * (We would need intersectNoPoints and intersectLineSegment, but these
#      are provided by GeometryValue and should not be overridden.)
#   *  intersectWithSegmentAsLineResult, which is used by 
#      intersectLineSegment as described in the assignment
#
# you can define other helper methods, but will not find much need to

# Note: geometry objects should be immutable: assign to fields only during
#       object construction

# Note: For eval_prog, represent environments as a dictionary whose key represents variable name
#       as described in the assignment

class GeometryExpression: 
    # do *not* change this class definition
    Epsilon = 0.00001

class GeometryValue:
    # do *not* change methods in this class definition
    # you can add methods if you wish

    # some helper methods that may be generally useful
    def real_close(self,r1,r2):
        return abs(r1-r2) < GeometryExpression.Epsilon

    def real_close_point(self,x1,y1,x2,y2):
        return self.real_close(x1, x2) and self.real_close(y1, y2) 
  
    # two_points_to_line could return a Line or a VerticalLine
    def two_points_to_line(self,x1,y1,x2,y2):
        if self.real_close(x1,x2):
            return VerticalLine(x1)
        else:
            m = float(y2 - y1) / (x2 - x1)
            b = y1 - m * x1
            return Line(m,b)

    # we put this in this class so all subclasses can inherit it:
    # the intersection of self with a NoPoints is a NoPoints object
    def intersectNoPoints(self,np):
        return np # could also have NoPoints() here instead

    # we put this in this class so all subclasses can inhert it:
    # the intersection of self with a LineSegment is computed by
    # first intersecting with the line containing the segment and then
    # calling the result's intersectWithSegmentAsLineResult with the segment
    def intersectLineSegment(self, seg):
        line_result = self.intersect(self.two_points_to_line(seg.x1,seg.y1,seg.x2,seg.y2))
        return line_result.intersectWithSegmentAsLineResult(seg)

class NoPoints(GeometryValue):
  # do *not* change this class definition: everything is done for you
  # (although this is the easiest class, it shows what methods every subclass
  # of geometry values needs)

  # Note: no __init__ method only because there is nothing it needs to do
  def eval_prog(self,env):
      return self # all values evaluate to self

  def preprocess_prog(self):
      return self # no pre-processing to do here

  def shift(self,dx,dy):
      return self # shifting no-points is no-points

  def intersect(self,other):
      return other.intersectNoPoints(self) # will be NoPoints but follow double-dispatch

  def intersectPoint(self,p):
      return self # intersection with point and no-points is no-points

  def intersectLine(self,line):
      return self # intersection with line and no-points is no-points

  def intersectVerticalLine(self,vline):
      return self # intersection with line and no-points is no-points

  # if self is the intersection of (1) some shape s and (2) 
  # the line containing seg, then we return the intersection of the 
  # shape s and the seg.  seg is an instance of LineSegment
  def intersectWithSegmentAsLineResult(self,seg):
      return self





class Point(GeometryValue):
  # *add* methods to this class -- do *not* change given code and do not
  # override any methods

  # Note: You may want a private helper method like the local
  # helper function inbetween in the ML code
  def __init__(self,x,y):
      self.x = x
      self.y = y


  def eval_prog(self,env):
      return self 

  def preprocess_prog(self):
      return self

  def shift(self,dx,dy):
      return Point(self.x+dx, self.y+dy)

  def intersect(self,other):
      return other.intersectPoint(self)

  def intersectPoint(self,p):
      if self.real_close_point(self.x, self.y, p.x, p.y):
          return self
      else :
          return NoPoints()

  def intersectLine (self,l):
      if self.real_close(self.y, l.m*self.x+l.b):
          return self
      else :
          return NoPoints()

  def intersectVerticalLine (self,vl):
      if self.real_close(self.x, vl.x) :
          return self
      else :
          return NoPoints()

  def intersectWithSegmentAsLineResult (self,seg):
      if self.inbetween(self.x, seg.x1, seg.x2) and self.inbetween(self.y, seg.y1, seg.y2):
          return Point(self.x, self.y)
      else :
          return NoPoints()

  def inbetween(self,v,end1,end2):
      return (end1-GeometryExpression.Epsilon<=v and v<=end2+GeometryExpression.Epsilon) or (end2-GeometryExpression.Epsilon<=v and v<=end1+GeometryExpression.Epsilon)




class Line(GeometryValue):
  # *add* methods to this class -- do *not* change given code and do not
  # override any methods
  def __init__(self,m,b):
      self.m = m
      self.b = b


  def eval_prog(self,env):
      return self 

  def preprocess_prog(self):
      return self 

  def shift(self,dx,dy):
      return Line(self.m, self.b+dy-self.m*dx)

  def intersect(self,other):
      return other.intersectLine(self)

  def intersectPoint(self,p):
      return p.intersectLine(self)

  def intersectLine (self,l):
      if self.real_close(self.m, l.m):
          if self.real_close(self.b, l.b):
              return self
          else:
              return NoPoints()
      else:
          return Point((l.b-self.b)/(self.m-l.m), self.m*((l.b-self.b)/(self.m-l.m))+self.b )

  def intersectVerticalLine (self,vl):
      return Point(vl.x, self.m*vl.x+self.b)

  def intersectWithSegmentAsLineResult (self,seg):
      return seg




class VerticalLine(GeometryValue):
    # *add* methods to this class -- do *not* change given code and do not
    # override any methods
    def __init__(self,x):
        self.x = x


    def eval_prog(self,env):
        return self

    def preprocess_prog(self):
        return self 

    def shift(self,dx,dy):
        return VerticalLine(self.x+dx)

    def intersect(self,other):
        return other.intersectVerticalLine(self)

    def intersectPoint(self,p):
        return p.intersectVerticalLine(self)

    def intersectLine (self,l):
        return l.intersectVerticalLine(self)

    def intersectVerticalLine (self,vl):
        if self.real_close(self.x, vl.x):
             return self
        else :
            return NoPoints()

    def intersectWithSegmentAsLineResult(self,seg):
        return seg




class LineSegment(GeometryValue):
    # *add* methods to this class -- do *not* change given code and do not
    # override any methods
    # Note: This is the most difficult class.  In the sample solution,
    #  preprocess_prog is about 15 lines long and 
    # intersectWithSegmentAsLineResult is about 40 lines long
    def __init__(self,x1,y1,x2,y2):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2


    def eval_prog(self,env):
        return self 

    def preprocess_prog(self):
        if self.real_close_point(self.x1, self.y1, self.x2, self.y2):
            return Point(self.x1, self.y1)
        elif self.real_close(self.x1, self.x2) and self.y2<self.y1 or self.x2<self.x1:
            return LineSegment(self.x2, self.y2, self.x1, self.y1)
        else:
            return self

    def shift(self,dx,dy):
        return LineSegment(self.x1+dx, self.y1+dy, self.x2+dx, self.y2+dy)

    def intersect (self,other):
        return other.intersectLineSegment(self)

    def intersectPoint (self,p):
        return p.intersectLineSegment(self)

    def intersectLine(self,l):
        return l.intersectLineSegment(self)

    def intersectVerticalLine(vl):
        return vl.intersectLineSegment(self)

    def intersectWithSegmentAsLineResult(self,seg):
        if self.real_close(self.x1, seg.x1) and self.real_close(self.x2, seg.x2):
            return self
        elif self.real_close(self.x1, seg.x1):
            if self.x2<seg.x2 :
                return self
            else :
                return LineSegment(self.x1, self.y1, seg.x2, seg.y2)
        elif self.real_close(self.x2, seg.x2):
            if self.x1<seg.x1 :
                return LineSegment(seg.x1, seg.y1, self.x2, self.y2)
            else :
                return self
        elif (self.x2<seg.x1) or (self.x1>seg.x2) :
            return NoPoints()
        else :
            if self.x1<seg.x1:
                maxx1=seg.x1
            else :
                maxx1=self.x1
            if self.y1<seg.y1:
                maxy1=seg.y1
            else :
                maxy1=self.y1
            if self.x2<seg.x2:
                minx2=self.x2
            else :
                minx2=seg.x2
            if self.y2<seg.y2:
                miny2=self.y2
            else :
                miny2=seg.y2
            return LineSegment(maxx1, maxy1, minx2, miny2)




class Intersect(GeometryExpression):
    # *add* methods to this class -- do *not* change given code and do not
    # override any methods
    def __init__(self,e1,e2):
        self.e1 = e1
        self.e2 = e2


    def eval_prog(self,env):
        return self.e1.eval_prog(env).intersect(self.e2.eval_prog(env)) 

    def preprocess_prog(self):
        return Intersect(self.e1.preprocess_prog(), self.e2.preprocess_prog())




class Let(GeometryExpression):
    # *add* methods to this class -- do *not* change given code and do not
    # override any methods
    # Note: Look at Var to guide how you implement Let
    def __init__(self,s,e1,e2):
        self.s = s
        self.e1 = e1
        self.e2 = e2


    def eval_prog(self,env):
        env[self.s]=self.e1.eval_prog(env)
        return self.e2.eval_prog(env)

    def preprocess_prog(self):
        return Let(self.s, self.e1.preprocess_prog(), self.e2.preprocess_prog())




class Var(GeometryExpression):
    # *add* methods to this class -- do *not* change given code and do not
    # override any methods
    def __init__(self,s):
        self.s = s

    def eval_prog(self,env): # remember: do not change this method
        try:
            return env[self.s]
        except:
            raise Exception("Undefined variable["+self.s+"]")


    def preprocess_prog(self):
        return self 




class Shift(GeometryExpression):
    # *add* methods to this class -- do *not* change given code and do not
    # override any methods
    def __init__(self,dx,dy,e):
        self.dx = dx
        self.dy = dy
        self.e = e


    def eval_prog(self,env):
        return self.e.eval_prog(env).shift(self.dx, self.dy)

    def preprocess_prog(self):
        return Shift(self.dx, self.dy, self.e.preprocess_prog())
