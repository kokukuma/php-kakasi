<?php
/**
 * Sample of php-kakasi. 
 *
 * 2011/12/08 written by karino 
 */

    $wordset = KAKASI_MORPHEME("狩野達也君"); 
	var_dump($wordset);

    $wordset = KAKASI_CONVERT("狩野たつやクン"); 
	var_dump($wordset);

?>
