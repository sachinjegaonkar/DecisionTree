REM
REM demonstration of how to run DrawTag
REM you many need to change this depending on where your Java 1.4 runtime is
REM
PATH=%PATH%;"C:\\Program Files\\Java\\jdk1.6.0\\bin"
java -Xmx256m -classpath ".";"C:\\Program Files\\Java\\jdk1.6.0\\jre\\lib" DrawTag %1.xml %1.jpg
