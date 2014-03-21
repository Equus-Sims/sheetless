<!DOCTYPE html>
<html>
    <head>
		<title>Much a do-while</title>
	</head>
	<body>
    <?php
		$loopCond = false;
		do {
			echo "<p>The loop ran even though the loop condition is false.</p>";
	}	while ($loopCond == true);
	
		echo "<p>Now the loop is done running.</p>";
    ?>
    </body>
</html>
