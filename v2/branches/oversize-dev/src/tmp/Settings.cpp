#include "Settings.h"// Constructors/Destructors//  Settings::Settings ( ) {initAttributes();}Settings::~Settings ( ) { }//  // Methods//  // Accessor methods//  // Private static attribute accessor methods//  /** * Get the value of m_mSelf * @return the value of m_mSelf */Settings* Settings::getMSelf ( ) {	return Settings::m_mSelf;}// Private attribute accessor methods//  /** * Set the value of m_mMaxRecentFiles * @param new_var the new value of m_mMaxRecentFiles */void Settings::setMMaxRecentFiles ( int new_var ) {	m_mMaxRecentFiles = new_var;}/** * Get the value of m_mMaxRecentFiles * @return the value of m_mMaxRecentFiles */int Settings::getMMaxRecentFiles ( ) {	return m_mMaxRecentFiles;}/** * Set the value of m_mMaxRecentProjects * @param new_var the new value of m_mMaxRecentProjects */void Settings::setMMaxRecentProjects ( int new_var ) {	m_mMaxRecentProjects = new_var;}/** * Get the value of m_mMaxRecentProjects * @return the value of m_mMaxRecentProjects */int Settings::getMMaxRecentProjects ( ) {	return m_mMaxRecentProjects;}// Other methods//  /** * @return int */int Settings::maxRecentFiles ( ) const {

}/** * @param  i */void Settings::setMaxRecentFiles (int i ) {

}/** * @return int */int Settings::maxRecentProjects ( ) const {

}/** * @param  i */void Settings::setMaxRecentProjects (int i ) {

}/** */void Settings::languages ( ) {

}/** * @return Settings* */Settings* Settings::current ( ) {

}void Settings::initAttributes ( ) {	m_mMaxRecentFiles = 15;	m_mMaxRecentProjects = 15;	m_mSelf = 0L;}