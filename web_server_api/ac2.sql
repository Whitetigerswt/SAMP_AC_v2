-- phpMyAdmin SQL Dump
-- version 4.0.4.1
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Jan 19, 2016 at 03:57 PM
-- Server version: 5.5.46-cll-lve
-- PHP Version: 5.4.45

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `ac2`
--
CREATE DATABASE IF NOT EXISTS `ac2` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `ac2`;

-- --------------------------------------------------------

--
-- Table structure for table `bans`
--

CREATE TABLE IF NOT EXISTS `bans` (
  `SQLid` int(128) NOT NULL AUTO_INCREMENT,
  `Cheater` varchar(128) NOT NULL,
  `CheaterIP` varchar(128) NOT NULL,
  `Hardware` varchar(128) NOT NULL,
  `Reason` varchar(128) NOT NULL,
  `ServerIP` varchar(128) NOT NULL,
  `ServerName` varchar(128) NOT NULL,
  `DateTime` varchar(128) NOT NULL,
  PRIMARY KEY (`SQLid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
