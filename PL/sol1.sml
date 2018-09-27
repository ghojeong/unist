(*20131674    Gho JeongWan*)
(*CSE34101*)

fun merge (list1 : int list, list2 : int list) =
	if null list1
	then list2
	else if null list2
	then list1
	else
		if hd list1 < hd list2
		then hd(list1) :: merge(tl(list1), list2)
		else hd(list2) :: merge(list1, tl(list2))

fun reverse (lst:int list) =
	case lst of
		[] => []
		| head::tail => (reverse tail) @ [head]

fun sigma (a: int, b: int, f: (int->int)) =
	if a>b
	then 0
	else f(a)+sigma(a+1, b, f)

fun digits (n: int) =
	if n<1
	then []
	else digits( n div 10 ) @ [n mod 10]

fun additivePersistence (n: int)=
	let
		fun sum(nlst: int list) =
			if null nlst
			then 0
			else hd nlst + sum(tl nlst)
		val lst=digits(n)
	in
		if null (tl lst)
		then 0
		else 1+additivePersistence( sum(lst) )
	end

fun digitalRoot (n: int) =
	let
		fun sum(nlst: int list) =
			if null nlst
			then 0
			else hd nlst + sum(tl nlst)
		val lst=digits(n)
	in
		if null (tl lst)
		then hd lst
		else digitalRoot( sum(lst) )
	end

fun testf (n: int) =
	10*n
