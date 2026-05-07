-- MySQL dump 10.13  Distrib 8.0.26, for Linux (x86_64)
--
-- Host: localhost    Database: HelpDesk
-- ------------------------------------------------------
-- Server version	8.0.26-0ubuntu0.20.04.3

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Department`
--

DROP TABLE IF EXISTS `Department`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Department` (
  `id` int NOT NULL AUTO_INCREMENT,
  `departmentName` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Department`
--

LOCK TABLES `Department` WRITE;
/*!40000 ALTER TABLE `Department` DISABLE KEYS */;
INSERT INTO `Department` VALUES (1,'HR'),(2,'Design'),(3,'Production'),(4,'Accounting'),(5,'Purchasing'),(6,'Plan'),(7,'Management'),(8,'Support');
/*!40000 ALTER TABLE `Department` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Employee`
--

DROP TABLE IF EXISTS `Employee`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Employee` (
  `id` int NOT NULL AUTO_INCREMENT,
  `employeeName` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `workNumber` text COLLATE utf8mb4_unicode_ci,
  `employeeDepartment` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `employeeLevel` int DEFAULT NULL,
  `employeeTitle` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Employee`
--

LOCK TABLES `Employee` WRITE;
/*!40000 ALTER TABLE `Employee` DISABLE KEYS */;
INSERT INTO `Employee` VALUES (1,'Jack Lu','1022','Design',8,'Director'),(2,'WK Wang','2033','Accounting',6,'Manager'),(3,'KK Jiang','2012','HR',2,'Staff'),(4,'Wai LI','3021','Production',1,'Worker'),(5,'Administrator','1234','Accounting',9,'Director');
/*!40000 ALTER TABLE `Employee` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `HelpDesk_Team`
--

DROP TABLE IF EXISTS `HelpDesk_Team`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `HelpDesk_Team` (
  `id` int NOT NULL AUTO_INCREMENT,
  `userName` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `password` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `roleTitle` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `isActive` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `HelpDesk_Team`
--

LOCK TABLES `HelpDesk_Team` WRITE;
/*!40000 ALTER TABLE `HelpDesk_Team` DISABLE KEYS */;
INSERT INTO `HelpDesk_Team` VALUES (1,'Engineer1','1234','Engineer',1),(2,'Manager1','123','Manager',1),(3,'Engineer Two','123456','Engineer',0),(4,'Engineer3','1234','Engineer',1),(5,'Engineer4','1234','Engineer',1),(6,'Engineer5','1234','Engineer',1),(7,'Jack Ti','1111','Engineer',1),(8,'Manager2','321','Manager',0),(9,'Manager3','321','Manager',1),(12,'manager11','11223399999','Engineer',0),(13,'engineer8','1234','Engineer',1);
/*!40000 ALTER TABLE `HelpDesk_Team` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Level`
--

DROP TABLE IF EXISTS `Level`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Level` (
  `id` int NOT NULL AUTO_INCREMENT,
  `employeeLevel` int DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Level`
--

LOCK TABLES `Level` WRITE;
/*!40000 ALTER TABLE `Level` DISABLE KEYS */;
INSERT INTO `Level` VALUES (1,1),(2,2),(3,3),(4,4),(5,5),(6,6),(7,7),(8,8),(9,9),(10,10);
/*!40000 ALTER TABLE `Level` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Ticket`
--

DROP TABLE IF EXISTS `Ticket`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Ticket` (
  `id` int NOT NULL AUTO_INCREMENT,
  `employeeName` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `employeeDepartment` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `issueType` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `issueLevel` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `issueDescription` varchar(1000) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `issueCreateTime` datetime DEFAULT NULL,
  `issueStatus` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `solutionDescription` varchar(2000) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `issueSolvedTime` datetime DEFAULT NULL,
  `attachedFile` varchar(2000) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `itEngineer` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `remarkInformation` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=100159 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Ticket`
--

LOCK TABLES `Ticket` WRITE;
/*!40000 ALTER TABLE `Ticket` DISABLE KEYS */;
INSERT INTO `Ticket` VALUES (100002,'Hello World','Design','Hardware issue','Critical','Install Pro-EHardware issueHardware issueHardware issueHardware issueHardware issueHardware issueHardware issue','2022-01-05 08:49:28','Hold','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100003,'Hello World2','HR','Software issue','Critical','Install Pro-E','2022-01-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100004,'Hello World3','HR','Software issue','Critical','Install Pro-E','2022-01-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100007,'Hello World','HR','Hardware issue','Critical','Install Pro-E','2022-01-05 08:49:28','Closed','Install hardware','2022-03-05 08:49:28','No file','Jack Ti','Remark'),(100008,'Hello World2','HR','Software issue','Critical','Install Pro-E','2022-08-05 08:49:28','Hold','install software','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100009,'Hello World3','Design','Software issue','Critical','Install Pro-E','2022-02-05 08:49:28','Closed','install software','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100012,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2022-02-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100013,'Hello World2','Design','Software issue','Critical','Install Pro-E','2022-02-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100014,'Hello World3','Design','Software issue','Critical','Install Pro-E','2022-02-05 08:49:28','Hold','install software','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100017,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2022-02-05 08:49:28','Hold','Install hardware, test update','2022-03-05 08:49:28','No file','Manager2','Remark\nUpdate Time is Tue Aug 16 21:58:06 2022, updated case status to Hold and updated case solution with remark by IT Engineer: Engineer1\nUpdate Time is Tue Aug 16 21:58:21 2022, Handover case from IT Engineer: Engineer1 to IT Engineer: Manager2.\nUpdate Time is Tue Aug 16 21:58:27 2022, Closed this case from Case Status: Hold to Case Status: Closed and updated solution with remark by IT Engineer: Manager2.\nUpdate Time is Tue Aug 16 22:06:41 2022, updated case status to Closed and updated case solution with remark by IT Engineer: Engineer1'),(100018,'Hello World2','Purchasing','Software issue','Critical','Install Pro-E','2022-02-05 08:49:28','Closed','install software','2022-03-05 08:49:28','No file','Engineer5','Remark\nModify time is Tue Aug 16 20:50:02 2022, Update IT Engineer from Manager1 to Engineer1．\nUpdate time is Tue Aug 16 20:51:26 2022, Update IT Engineer from Engineer4 to Engineer1．'),(100019,'Hello World3','Purchasing','Software issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer1','TEst update records\nModify time is Tue Aug 16 16:35:55 2022, Update IT Engineer from Engineer1 to Manager1．\nModify time is Tue Aug 16 16:36:40 2022, Update IT Engineer from Manager1 to Manager1．\nModify time is Tue Aug 16 19:36:06 2022, Update IT Engineer from Manager1 to Engineer1．'),(100022,'Hello World','Safety','Hardware issue','Critical','Install Pro-E','2022-03-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark\nTime: \'Sun Aug 14 10:46:52 2022\', Update itEngineer from \'Engineer II\' to \'Engineer1\';'),(100024,'Hello World3','Safety','Software issue','Critical','Install Pro-E','2022-03-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100027,'Hello World','Purchasing','Hardware issue','Critical','Install Pro-E','2022-03-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','remarkResult'),(100028,'Hello World2','Purchasing','Software issue','Critical','Install Pro-E','2022-03-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer1','remarkResult\nUpdate time is Tue Aug 16 16:40:03 2022, Update IT Engineer from Engineer1 to Manager1．\nModify time is Tue Aug 16 16:48:14 2022, Update IT Engineer from Manager1 to Engineer1．'),(100029,'Hello World3','Design','Software issue','Critical','Install Pro-E','2022-03-05 08:49:28','Hold','install software','2022-03-05 08:49:28','No file','Engineer1','Remark\nModify time is Tue Aug 16 20:33:50 2022, Update IT Engineer from Manager1 to Engineer1．'),(100032,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2022-08-05 08:49:28','Hold','Install hardware','2022-03-05 08:49:28','No file','Engineer4','Remark\nUpdate Time is Tue Aug 16 21:55:08 2022, updated case status to Open and updated case solution with remark by IT Engineer: .'),(100033,'Hello World2','Design','Software issue','Critical','Install Pro-E','2022-02-05 08:49:28','Closed','test closed solution','2022-03-05 08:49:28','No file','Engineer4','test closed remark\nUpdate Time is Tue Aug 16 21:51:09 2022, Closed this case from Case Status: Closed to Case Status: Closed and updated solution with remark by IT Engineer: .'),(100034,'Hello World3','Design','Software issue','Critical','Install Pro-E','2022-02-05 08:49:28','Hold','install software','2022-03-05 08:49:28','No file','Manager1','test long text in sql'),(100037,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2022-02-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer5','Remark'),(100038,'Hello World2','Accounting','Software issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Manager1','Remark'),(100039,'Hello World3','Accounting','Software issue','Critical','Install Pro-E','2022-04-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Manager1','Remark'),(100042,'Hello World','Accounting','Hardware issue','Critical','Install Pro-E','2022-04-05 08:49:28','Hold','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100043,'Hello World2','Accounting','Software issue','Critical','Install Pro-E','2022-04-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Manager1','Remark'),(100044,'Hello World3','Accounting','Software issue','Critical','Install Pro-E','2022-04-05 08:49:28','Hold','install software','2022-03-05 08:49:28','No file','Engineer5','Remark'),(100047,'Hello World','Accounting','Hardware issue','Critical','Install Pro-E','2022-04-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer5','Remark'),(100048,'Hello World2','Accounting','Software issue','Critical','Install Pro-E','2022-04-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer5','Remark'),(100049,'Hello World3','Accounting','Software issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer5','Remark'),(100052,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer5','Remark'),(100053,'Hello World2','Design','Software issue','Critical','Install Pro-E','2022-05-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100054,'Hello World3','Engineering','Software issue','Critical','Install Pro-E','2022-05-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100057,'Hello World','Engineering','Hardware issue','Critical','Install Pro-E','2022-05-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer5','Remark'),(100058,'Hello World2','Engineering','Software issue','Critical','Install Pro-E','2022-05-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100059,'Hello World3','Design','Software issue','Critical','Install Pro-E','2022-05-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100062,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2022-04-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer5','Remark'),(100063,'Hello World2','Design','Software issue','Critical','Install Pro-E','2022-04-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100064,'Hello World3','Logistic','Software issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer3','Remark'),(100067,'Hello World','Logistic','Hardware issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer5','Remark'),(100068,'Hello World2','Logistic','Software issue','Critical','Install Pro-E','2022-07-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100069,'Hello World3','Logistic','Software issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100072,'Hello World','Logistic','Hardware issue','Critical','Install Pro-E','2022-06-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100073,'Hello World2','Design','Software issue','Critical','Install Pro-E','2022-06-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer4','Remark'),(100074,'Hello World3','Design','Software issue','Critical','Install Pro-E','2022-06-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer4','Remark'),(100077,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2022-06-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100078,'Hello World2','Management','Software issue','Critical','Install Pro-E','2022-06-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer4','Remark'),(100079,'Hello World3','Management','Software issue','Critical','Install Pro-E','2022-06-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer4','Remark'),(100082,'Hello World','Management','Hardware issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100083,'Hello World2','Management','Software issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer4','Remark'),(100084,'Hello World3','Management','Software issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100087,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2021-07-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100088,'Hello World2','Law','Software issue','Critical','Install Pro-E','2021-07-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100089,'Hello World3','Law','Software issue','Critical','Install Pro-E','2021-07-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100092,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2021-07-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100093,'Hello World2','Design','Software issue','Critical','Install Pro-E','2022-07-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100094,'Hello World3','Design','Software issue','Critical','Install Pro-E','2022-07-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100097,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2022-07-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100098,'Hello World2','Design','Software issue','Critical','Install Pro-E','2022-03-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100099,'Hello World3','CAD','Software issue','Critical','Install Pro-E','2022-03-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100102,'Hello World','CAD','Hardware issue','Critical','Install Pro-E','2022-03-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100103,'Hello World2','CAD','Software issue','Critical','Install Pro-E','2022-03-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Manager1','Remark'),(100104,'Hello World3','CAD','Software issue','Critical','Install Pro-E','2022-03-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Manager1','Remark'),(100107,'Hello World','CAD','Hardware issue','Critical','Install Pro-E','2022-03-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100108,'Hello World2','CAD','Software issue','Critical','Install Pro-E','2022-08-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Manager1','Remark'),(100109,'Hello World3','CAD','Software issue','Critical','Install Pro-E','2021-09-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Manager1','Remark'),(100112,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2021-09-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100113,'Hello World2','Design','Software issue','Critical','Install Pro-E','2021-09-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100114,'Hello World3','IT','Software issue','Critical','Install Pro-E','2021-09-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100117,'Hello World','IT','Hardware issue','Critical','Install Pro-E','2021-09-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100118,'Hello World2','IT','Software issue','Critical','Install Pro-E','2021-09-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Manager1','Remark'),(100119,'Hello World3','IT','Software issue','Critical','Install Pro-E','2021-10-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Manager1','Remark'),(100122,'Hello World','Design','Hardware issue','Critical','Install Pro-E','2021-09-05 08:49:28','Open','Install hardware','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100123,'Hello World2','Design','Software issue','Critical','Install Pro-E','2021-09-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer II','Remark'),(100124,'Hello World3','Design','Software issue','Critical','Install Pro-E','2021-09-05 08:49:28','Open','install software','2022-03-05 08:49:28','No file','Engineer1','Remark'),(100126,'Jack Li','Testing','Software issue','Normal','Install Pro-E','2021-09-05 08:49:28','Hold',NULL,'2022-03-05 08:49:28',NULL,'Engineer1',NULL),(100127,'TESt timestamp','Testing','Setup New pc','Normal','test','2021-09-05 08:49:28','Closed','Setup done','2022-03-05 08:49:28','no file','Engineer1','Remark'),(100128,'Test Datetime','Testing','test','Normal','Install Pro-E','2021-09-05 08:49:28','Open',NULL,'2022-03-05 08:49:28',NULL,'Engineer1',NULL),(100129,'Administrator','ITIT','Software installation','Middle','DEsciption text....','2021-09-05 08:49:28','Closed',NULL,NULL,NULL,'Manager1',NULL),(100130,'Administrator','ITIT','Software installation','Critical','22DEsciption text....','2021-10-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100131,'Administrator','ITIT','Software issue','Normal','22DEsciption text....','2021-10-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100132,'Administrator','ITIT','Hardware issue','Middle','22DEsciption text....','2021-10-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100133,'Administrator','ITIT','Email issue','Middle','setup software to solve email issue. thanks','2021-10-05 08:49:28','Closed',NULL,NULL,NULL,'UnOwned',NULL),(100134,'Administrator','Accounting','Data backup/issue','High','just fix hardware issue.','2022-08-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100135,'Administrator','Accounting','Network issue','Critical','my network has problem, pls doulbe check asap.','2022-08-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100136,'Administrator','Accounting','Network issue','Critical','my network has problem, pls doulbe check asap.','2022-08-05 18:49:28','Closed',NULL,NULL,NULL,'UnOwned',NULL),(100137,'Administrator','Accounting','Software Development','Critical','send failure test','2022-08-05 08:49:28','Closed',NULL,NULL,NULL,'UnOwned',NULL),(100138,'Administrator','Accounting','Software Development','Critical','send failure test','2022-08-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100139,'Administrator','Accounting','Telephone issue','High','tel issue.','2021-11-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100140,'Administrator','Accounting','Telephone issue','High','tel issue.','2021-11-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100141,'Administrator','Accounting','Telephone issue','High','tel issue.','2021-11-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100142,'Administrator','Accounting','Telephone issue','High','tel issue.','2021-11-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100143,'Administrator','Accounting','IT Training','Low','training','2021-11-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100144,'Administrator','Accounting','IT Training','Low','training','2021-11-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100145,'Administrator','Accounting','IT Training','Low','training2','2021-11-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100146,'Administrator','Accounting','IT Training','Low','training2','2021-11-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100147,'Administrator','Accounting','Email issue','Low','email','2021-12-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100148,'Administrator','Accounting','Data backup/issue','Middle','test databackup','2021-12-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100149,'Administrator','Accounting','Data backup/issue','Middle','test databackup','2021-12-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100150,'Administrator','Accounting','Data backup/issue','Middle','test databackup','2021-12-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100151,'Administrator','Accounting','OA Devices issue','Middle','sdflf\n\ndjflaf\n\n\n\n\n\n\n\njfdlasf\n\n\ntest\n\ntest\n\n\ntest','2021-12-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100152,'Administrator','Accounting','Hardware issue','High','hardware issue, pls check.','2021-12-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100153,'Administrator','Accounting','Email issue','Critical','read local settings file successful. greate job!','2022-08-05 08:09:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100154,'Administrator','Accounting','Software problem','Middle','accounting software was crashed.','2022-08-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100155,'Administrator','Accounting','Data backup/issue','Middle','test backup','2022-08-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100156,'Administrator','Accounting','Telephone issue','High','tel issue.','2021-12-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100157,'Administrator','Accounting','Email issue','Middle','test unowned.','2021-12-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL),(100158,'Administrator','Accounting','Network issue','Critical','unowned.','2021-12-05 08:49:28','Open',NULL,NULL,NULL,'UnOwned',NULL);
/*!40000 ALTER TABLE `Ticket` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Title`
--

DROP TABLE IF EXISTS `Title`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Title` (
  `id` int NOT NULL AUTO_INCREMENT,
  `employeeTitle` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Title`
--

LOCK TABLES `Title` WRITE;
/*!40000 ALTER TABLE `Title` DISABLE KEYS */;
INSERT INTO `Title` VALUES (1,'Worker'),(2,'Staff'),(3,'Engineer'),(4,'SuperVisor'),(5,'Manager'),(6,'Director'),(7,'VP'),(8,'President');
/*!40000 ALTER TABLE `Title` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `issueLevel`
--

DROP TABLE IF EXISTS `issueLevel`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `issueLevel` (
  `id` int NOT NULL AUTO_INCREMENT,
  `issueLevel` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `issueLevel`
--

LOCK TABLES `issueLevel` WRITE;
/*!40000 ALTER TABLE `issueLevel` DISABLE KEYS */;
INSERT INTO `issueLevel` VALUES (1,'Low'),(2,'Middle'),(3,'High'),(4,'Critical');
/*!40000 ALTER TABLE `issueLevel` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `issueStatus`
--

DROP TABLE IF EXISTS `issueStatus`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `issueStatus` (
  `id` int NOT NULL AUTO_INCREMENT,
  `issueStatusName` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `issueStatus`
--

LOCK TABLES `issueStatus` WRITE;
/*!40000 ALTER TABLE `issueStatus` DISABLE KEYS */;
INSERT INTO `issueStatus` VALUES (1,'Closed'),(2,'Open'),(3,'Hold');
/*!40000 ALTER TABLE `issueStatus` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `issueType`
--

DROP TABLE IF EXISTS `issueType`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `issueType` (
  `id` int NOT NULL AUTO_INCREMENT,
  `issueType` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `issueType`
--

LOCK TABLES `issueType` WRITE;
/*!40000 ALTER TABLE `issueType` DISABLE KEYS */;
INSERT INTO `issueType` VALUES (1,'Software problem'),(2,'Software installation'),(3,'Hardware issue'),(4,'Data backup/issue'),(5,'Email issue'),(6,'Network issue'),(7,'Software Development'),(8,'OA Devices issue'),(9,'Telephone issue'),(10,'IT Training'),(11,'Other issue');
/*!40000 ALTER TABLE `issueType` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-08-17 13:21:07
