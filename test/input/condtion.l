entier $n;
condition(entier $a, entier $b) 
entier $i;
{
	$i = !$b;
	retour  $i;
}

main() {
	ecrire(condition(0,0));
	ecrire(condition(0,1));
	ecrire(condition(1,0));
	ecrire(condition(1,1));
}