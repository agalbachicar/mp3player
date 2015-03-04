name := "music_server"

version := "1.0-SNAPSHOT"

scalaVersion := "2.10.3" 

libraryDependencies ++= Seq(
  javaJdbc,
  javaEbean,
  cache,
  "mysql" % "mysql-connector-java" % "5.1.18",
  "org.apache.tika" % "tika-core" % "1.4",
  "org.apache.tika" % "tika-parsers" % "1.4"
)     

play.Project.playJavaSettings
