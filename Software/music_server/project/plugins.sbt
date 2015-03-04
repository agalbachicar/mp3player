// Comment to get more information during initialization
logLevel := Level.Warn

// The Typesafe repository
resolvers += "Typesafe repository" at "http://repo.typesafe.com/typesafe/releases/"

// Use the Play sbt plugin for Play projects
addSbtPlugin("com.typesafe.play" % "sbt-plugin" % "2.2.2")

libraryDependencies ++= Seq("mysql" % "mysql-connector-java" % "5.1.29")
//libraryDependencies ++= Seq("org.apache.tika" % "tika-core" % "1.4")
//libraryDependencies ++= Seq("org.apache.tika" % "tika-parsers" % "1.4")