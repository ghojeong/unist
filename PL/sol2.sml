(*20131674    Gho JeongWan*)
(*CSE34101*)
(*Problem 1*)
datatype expr = NUM of int
		| PLUS of expr * expr
		| MINUS of expr * expr
datatype formula = TRUE
		| FALSE
		| NOT of formula
		| ANDALSO of formula * formula
		| ORELSE of formula * formula
		| IMPLY of formula * formula
		| LESS of expr * expr
fun eval e =
	let
		fun getExpr e =
			case e of
				  NUM (i) => i
				| PLUS (ex1, ex2) => (getExpr ex1) + (getExpr ex2)
				| MINUS (ex1, ex2) => (getExpr ex1) - (getExpr ex2)
	in
		case e of
			  TRUE => true
			| FALSE => false
			| NOT e1 => not (eval e1)
			| ANDALSO (e1, e2) => (eval e1) andalso (eval e2)
			| ORELSE (e1, e2) => (eval e1) orelse (eval e2)
			| IMPLY (e1, e2) => (not (eval e1)) orelse (eval e2)
			| LESS (ex1, ex2) => (getExpr ex1) < (getExpr ex2)
	end
(* Problem 2 *)
type name = string
datatype metro = STATION of name
		| AREA of name * metro
		| CONNECT of metro * metro
fun checkMetro metro1 =
	let
		fun checkString (name1, nlist) =
			case nlist of
				  [] => false
				| hd1::tl1 => (name1 = hd1) orelse (checkString(name1, tl1))
		fun check (metro2, nlist) =
			case metro2 of
				   STATION (name2) => (checkString (name2, nlist)) 
				| AREA (name3, metro3) => ( check(metro3, name3::nlist) )
				| CONNECT (metro4, metro5) => (check(metro4,nlist)) andalso (check(metro5,nlist))
	in
		check(metro1,[])
	end
(*problem 3*)
(*i*)
datatype 'a lazyList = nullList
		| cons of 'a * (unit -> 'a lazyList)
fun seq (first, last) =
	if (first <= last)
	then cons (first, (fn () => seq (first+1, last)) )
	else nullList
fun infSeq (first) = cons (first, (fn () => infSeq (first+1)) )
fun firstN (lazyListVal, n) = 
	if ( n > 0)
	then case lazyListVal of
		   nullList => []
		| cons (first, f) => first :: firstN (f(), n-1)
	else []		
fun Nth (lazyListVal, n) =
	if ( n > 1)
	then case lazyListVal of
		   nullList => NONE
		| cons (first, f) => Nth (f(), n-1)
	else case lazyListVal of
		   nullList => NONE
		| cons (first, f) => SOME first
fun filterMultiples (lazyListVal, n) =
	case lazyListVal of
		  nullList => nullList
		| cons (first, f) =>
			if ( (first mod n) = 0 )
			then case f() of
				   nullList => nullList
				| cons (first2, f2) => cons (first2, (fn () => filterMultiples (f2(),n)) )
			else  cons (first, (fn () => filterMultiples (f(),n)) )
(*ii*)
fun sieve (lazyListVal) =
	case lazyListVal of
		   nullList => nullList
		| cons (first, f) => cons (first, (fn () => sieve(filterMultiples(f(), first))))
fun primes () = sieve ( infSeq(2) )
