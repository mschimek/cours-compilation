entier $tab[10];

initialiser()
{
    $tab[0] = 8;    $tab[1] = 6;    $tab[2] = 9;
    $tab[3] = 9;    $tab[4] = 4;    $tab[5] = 2;
    $tab[6] = 3;    $tab[7] = 1;    $tab[8] = 4;
    $tab[9] = 5;    
}

afficher( entier $n )
entier $i;
{
    $i = 0;
    tantque $i < $n faire {
        ecrire( $tab[ $i ] );
        $i = $i + 1;
    }    
    ecrire( 0 ); # marqueur fin de tableau         
}

tri(entier $debut, entier $fin)
entier $pivot,
entier $left,
entier $right,
entier $tmp;
{
     #afficher(10);
     #ecrire(0-1);
     #ecrire($debut);
     #ecrire(0-1);
     #ecrire($fin);
     #ecrire(0-1);
	si $debut < $fin alors {
		$pivot = $tab[$debut];
		$left = $debut;
		$right= $fin;
		tantque $left < ($right + 1) faire 
		{
			tantque ($tab[$left] < $pivot) faire 
			{
				$left = $left + 1;
			}
			tantque (($pivot < $tab[$right]) & (($debut -1) <  $right))  faire {
				$right = $right - 1;
			}
			si $left < ($right + 1)alors
			{
				$tmp = $tab[$right];
				$tab[$right] = $tab[$left];
				$tab[$left] = $tmp;
				$right = $right -1;
				$left = $left + 1;
			}
		}
		tri($debut, $right);
		tri($left, $fin);
	}
	
	
}

main() {
	initialiser();
	afficher(3);
	tri(0,2);
	afficher(3);
}
