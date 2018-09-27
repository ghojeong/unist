(*20131674    Gho JeongWan*)
(*CSE34101*)

exception NoAnswer

datatype pattern = Wildcard
		 | Variable of string
		 | UnitP
		 | ConstP of int
		 | TupleP of pattern list
		 | ConstructorP of string * pattern

datatype valu = Const of int
	      | Unit
	      | Tuple of valu list
	      | Constructor of string * valu

fun g f1 f2 p =
    let 
	val r = g f1 f2 
    in
	case p of
	    Wildcard          => f1 ()
	  | Variable x        => f2 x
	  | TupleP ps         => List.foldl (fn (p,i) => (r p) + i) 0 ps
	  | ConstructorP(_,p) => r p
	  | _                 => 0
    end

datatype typ = Anything
	     | UnitT
	     | IntT
	     | TupleT of typ list
	     | Datatype of string

(**** you can put all your code here ****)

fun shortest_string1 strList =
	List.foldl (fn (x,acc) => if size(x)<size(acc) then x else acc) (hd strList) (tl strList)

fun shortest_string2 strList =
	List.foldl (fn (x,acc) => if size(x)>size(acc) then acc else x) (hd strList) (tl strList)

fun shortest_capitalized strList =
	let
		fun g (x, acc) =
			if Char.isUpper( String.sub (x, 0) )
			then ( if acc="" then (x,x) else (x,acc) )
			else (if acc="" then (x,acc) else (acc, acc))
		fun f (x,acc) = if size(x) > size(acc) then acc else x
	in
		List.foldl (f o g) "" strList
	end



fun check_pat pat =
	let
		fun merge (patt, strList) = 
			case patt of
				Variable x => strList @ [x]
				| TupleP ps => (
					case ps of
					[] => strList
					| p::ps'=>merge(p,strList)@merge(TupleP ps',[])
					)
				| ConstructorP(_,p) => merge (p, strList)
				| _  => strList
		fun help1 patt =
			case patt of
				Variable x => [x]
				| TupleP ps => List.foldl merge [] ps
				| ConstructorP(_,p) => help1 p
				| _  => []

		fun checkExist (strList, str1) = List.exists (fn str2 => str1=str2) strList
		fun help2 strList =
			case strList of
				[] => false
				| s::ss => checkExist(ss, s) orelse help2(ss)
	in
		not ((help2 o help1) pat)
	end
		

fun  match (v,p) =
	case p of
		Wildcard => SOME []
		 | Variable s => SOME [(s,v)]
		 | UnitP => (
			case v of 
				Unit => SOME []
				| _ => NONE )
		 | ConstP n => (
			case v of
				 Const m => if n=m then SOME [] else NONE
				| _ => NONE )
		 | TupleP ps => (
			case v of
				Tuple vs => (
					let
						val vpList=ListPair.zip(vs,ps)
						fun isMatch (vpL: (valu*pattern) list) = (
							case vpL of
								[] => true
								| head::tail => isSome( match(#1 head, #2 head) ) andalso isMatch(tail)
						)
						fun  binding (vpL: (valu*pattern) list) = (
							case vpL of
								[] => []
								| head::tail => valOf( match(#1 head, #2 head) ) @ binding(tail)
						)
					in
						if isMatch(vpList) then SOME(binding vpList) else NONE
					end
				)
				| _ => NONE )
		 | ConstructorP (s1,pa) => (
			case v of 
				Constructor (s2, va) =>
					if (s1=s2) andalso isSome(match(va, pa))
					then match(va, pa)  else NONE
				| _ => NONE )

	
