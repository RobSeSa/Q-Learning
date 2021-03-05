datatype Tree<T> = Leaf | Node(Tree<T>, Tree<T>, T)
datatype List<T> = Nil | Cons(T, List<T>)

function flatten<T>(tree:Tree<T>):List<T>
{
	
}

function append<T>(xs:List<T>, ys:List<T>):List<T>
{
	match(xs)
        case Nil => ys
        case Cons(x', xs') => Cons(x', append(xs', ys))
}

function treeContains<T>(tree:Tree<T>, element:T):bool
{
	
}

function listContains<T>(xs:List<T>, element:T):bool
{
	
}


lemma sameElements<T>(tree:Tree<T>, element:T)
ensures treeContains(tree, element) <==> listContains(flatten(tree), element)
{
	
}