#ifndef __Configuration_H__
#define __Configuration_H__

		typedef enum
		{
			eMessageTypeBegin = 0,

			eSystemYourId,
			eSystemText,			

			eText,
			eMail,

			eAudio,
			eVideo,

			eFile,
			eFilePort,

			eUserList,
			eUserInfo,

			eMessageTypeEnd
			
		} MessageType;

#endif
