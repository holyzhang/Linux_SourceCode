/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009-2012 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/**
 * @file ump_ukk_wrappers.c
 * Defines the wrapper functions which turn Linux IOCTL calls into _ukk_ calls
 */

#include <asm/uaccess.h>             /* user space access */

#include "ump_osk.h"
#include "ump_uk_types.h"
#include "ump_ukk.h"
#include "ump_kernel_common.h"

/*
 * IOCTL operation; Negotiate version of IOCTL API
 */
int ump_get_api_version_wrapper(u32 __user * argument, struct ump_session_data * session_data)
{
	_ump_uk_api_version_s version_info;
	_mali_osk_errcode_t err;

	/* Sanity check input parameters */
	if (NULL == argument || NULL == session_data)
	{
		MSG_ERR(("NULL parameter in ump_ioctl_get_api_version()\n"));
		return -ENOTTY;
	}

	/* Copy the user space memory to kernel space (so we safely can read it) */
	if (0 != copy_from_user(&version_info, argument, sizeof(version_info)))
	{
		MSG_ERR(("copy_from_user() in ump_ioctl_get_api_version()\n"));
		return -EFAULT;
	}

	version_info.ctx = (void*) session_data;
	err = _ump_uku_get_api_version( &version_info );
	if( _MALI_OSK_ERR_OK != err )
	{
		MSG_ERR(("_ump_uku_get_api_version() failed in ump_ioctl_get_api_version()\n"));
		return map_errcode(err);
	}

	version_info.ctx = NULL;

	/* Copy ouput data back to user space */
	if (0 != copy_to_user(argument, &version_info, sizeof(version_info)))
	{
		MSG_ERR(("copy_to_user() failed in ump_ioctl_get_api_version()\n"));
		return -EFAULT;
	}

	return 0; /* success */
}


/*
 * IOCTL operation; Release reference to specified UMP memory.
 */
int ump_release_wrapper(u32 __user * argument, struct ump_session_data  * session_data)
{
	_ump_uk_release_s release_args;
	_mali_osk_errcode_t err;

	/* Sanity check input parameters */
	if (NULL == session_data)
	{
		MSG_ERR(("NULL parameter in ump_ioctl_release()\n"));
		return -ENOTTY;
	}

	/* Copy the user space memory to kernel space (so we safely can read it) */
	if (0 != copy_from_user(&release_args, argument, sizeof(release_args)))
	{
		MSG_ERR(("copy_from_user() in ump_ioctl_get_api_version()\n"));
		return -EFAULT;
	}

	release_args.ctx = (void*) session_data;
	err = _ump_ukk_release( &release_args );
	if( _MALI_OSK_ERR_OK != err )
	{
		MSG_ERR(("_ump_ukk_release() failed in ump_ioctl_release()\n"));
		return map_errcode(err);
	}


	return 0; /* success */
}

/*
 * IOCTL operation; Return size for specified UMP memory.
 */
int ump_size_get_wrapper(u32 __user * argument, struct ump_session_data  * session_data)
{
	_ump_uk_size_get_s user_interaction;
	_mali_osk_errcode_t err;

	/* Sanity check input parameters */
	if (NULL == argument || NULL == session_data)
	{
		MSG_ERR(("NULL parameter in ump_ioctl_size_get()\n"));
		return -ENOTTY;
	}

	if (0 != copy_from_user(&user_interaction, argument, sizeof(user_interaction)))
	{
		MSG_ERR(("copy_from_user() in ump_ioctl_size_get()\n"));
		return -EFAULT;
	}

	user_interaction.ctx = (void *) session_data;
	err = _ump_ukk_size_get( &user_interaction );
	if( _MALI_OSK_ERR_OK != err )
	{
		MSG_ERR(("_ump_ukk_size_get() failed in ump_ioctl_size_get()\n"));
		return map_errcode(err);
	}

	user_interaction.ctx = NULL;

	if (0 != copy_to_user(argument, &user_interaction, sizeof(user_interaction)))
	{
		MSG_ERR(("copy_to_user() failed in ump_ioctl_size_get()\n"));
		return -EFAULT;
	}

	return 0; /* success */
}

/*
 * IOCTL operation; Do cache maintenance on specified UMP memory.
 */
int ump_msync_wrapper(u32 __user * argument, struct ump_session_data  * session_data)
{
	_ump_uk_msync_s user_interaction;

	/* Sanity check input parameters */
	if (NULL == argument || NULL == session_data)
	{
		MSG_ERR(("NULL parameter in ump_ioctl_size_get()\n"));
		return -ENOTTY;
	}

	if (0 != copy_from_user(&user_interaction, argument, sizeof(user_interaction)))
	{
		MSG_ERR(("copy_from_user() in ump_ioctl_msync()\n"));
		return -EFAULT;
	}

	user_interaction.ctx = (void *) session_data;

	_ump_ukk_msync( &user_interaction );

	user_interaction.ctx = NULL;

	if (0 != copy_to_user(argument, &user_interaction, sizeof(user_interaction)))
	{
		MSG_ERR(("copy_to_user() failed in ump_ioctl_msync()\n"));
		return -EFAULT;
	}

	return 0; /* success */
}
