
#include "form.h"

XHtmlElements::Form::Form() :
	XHtmlElement()
{
	setType( "FORM" );
}

XHtmlElements::Form::~Form()
{}

QString XHtmlElements::Form::toHtml()
{
	QString asHtml = QString("<form %1").arg( getBasicAttributes() );
	
	if( false == _onChange.isEmpty() )
		asHtml += QString(" onChange=\"%1\" ").arg( _onChange );
	if( false == _onSubmit.isEmpty() )
		asHtml += QString(" onSubmit=\"%1\" ").arg( _onSubmit );
	if( false == _onReset.isEmpty() )
		asHtml += QString(" onReset=\"%1\" ").arg( _onReset );
	if( false == _onSelect.isEmpty() )
		asHtml += QString(" onSelect=\"%1\" ").arg( _onSelect );
	if( false == _onBlur.isEmpty() )
		asHtml += QString(" onBlur=\"%1\" ").arg( _onBlur );
	if( false == _onFocus.isEmpty() )
		asHtml += QString(" onFocus=\"%1\" ").arg( _onFocus );

	if( false == _action.isEmpty() )
		asHtml += QString(" action=\"%1\" ").arg( _action );
	if( false == _accept.isEmpty() )
		asHtml += QString(" accept=\"%1\" ").arg( _accept );
	if( false == _acceptCharset.isEmpty() )
		asHtml += QString(" acceptCharset=\"%1\" ").arg( _acceptCharset );
	if( false == _enctype.isEmpty() )
		asHtml += QString(" enctype=\"%1\" ").arg( _enctype );
	if( false == _method.isEmpty() )
		asHtml += QString(" method=\"%1\" ").arg( _method );
	if( false == _name.isEmpty() )
		asHtml += QString(" name=\"%1\" ").arg( _name );
	if( false == _target.isEmpty() )
		asHtml += QString(" target=\"%1\" ").arg( _target );

	asHtml+=">"

	asHtml+= getContentAsHtml();
	
	asHtml += "</form> ";
	
	return asHtml;
}

QString XHtmlElements::Form::getOnChange(){ return _onChange; }
QString XHtmlElements::Form::getOnSubmit(){ return _onSubmit; }
QString XHtmlElements::Form::getOnReset(){ return _onReset; }
QString XHtmlElements::Form::getOnSelect(){ return _onSelect; }
QString XHtmlElements::Form::getOnBlur(){ return _onBlur; }
QString XHtmlElements::Form::getOnFocus(){ return _onFocus; }
void    XHtmlElements::Form::setOnChange( QString onChange ){ _onChange = onChange; }
void    XHtmlElements::Form::setOnSubmit( QString onSubmit ){ _onSubmit = _onSubmit; }
void    XHtmlElements::Form::setOnReset( QString onReset ){ _onReset = _onReset; }
void    XHtmlElements::Form::setOnSelect( QString onSelect ){ _onSelect = _onSelect; }
void    XHtmlElements::Form::setOnBlur( QString onBlur ){ _onBlur = _onBlur; }
void    XHtmlElements::Form::setOnFocus( QString onFocus ){ _onFocus = _onFocus; }

QString XHtmlElements::Form::getAction(){ return _action; }
QString XHtmlElements::Form::getAccept(){ return _accept; }
QString XHtmlElements::Form::getAcceptCharset(){ return _acceptCharset; }
QString XHtmlElements::Form::getEnctype(){ return _enctype; }
QString XHtmlElements::Form::getMethod(){ return _method; }
QString XHtmlElements::Form::getName(){ return _name; }
QString XHtmlElements::Form::getTarget(){ return _target; }
void    XHtmlElements::Form::setAction( QString action ){ _action = action; }
void    XHtmlElements::Form::setAccept( QString accept ){ _accept = accept; }
void    XHtmlElements::Form::setAcceptCharset( QString acceptCharset ){ _acceptCharset = acceptCharset; }
void    XHtmlElements::Form::setEnctype( QString enctype ){ _enctype = enctype; }
void    XHtmlElements::Form::setMethod( QString method ){ _method = method; }
void    XHtmlElements::Form::setName( QString name ){ _name = name; }
void    XHtmlElements::Form::setTarget( QString type ){ _type = type; }
