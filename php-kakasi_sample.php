<?php
/**
 * Sample of php-kakasi. 
 *
 * 2011/12/08 written by karino 
 */

	$kakasi = new KAKASI();
	$kakasi->word = "キャプテン翼";

	$wordset = $kakasi->reproc();

	echo "元キーワード：".$wordset->base."\n";
	echo "ひらがな変換：".$wordset->hira."\n";
	echo "カタカナ変換：".$wordset->kata."\n";
	echo "Alphbet変換：".$wordset->alph."\n";

?>
