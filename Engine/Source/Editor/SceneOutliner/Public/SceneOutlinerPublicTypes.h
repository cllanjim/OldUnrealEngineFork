// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SceneOutlinerFwd.h"

#include "SceneOutlinerFilters.h"
#include "ISceneOutlinerColumn.h"
#include "ISceneOutliner.h"

enum class ESceneOutlinerMode : uint8
{
	/** Allows all actors to be browsed and selected; syncs selection with the editor; drag and drop attachment, stc. */
	ActorBrowsing,

	/** Sets the outliner to operate as an actor 'picker'. */
	ActorPicker,
};

namespace SceneOutliner
{
	/** Container for built in column types. Function-static so they are available without linking */
	struct FBuiltInColumnTypes
	{
		/** The gutter column */
		static const FName& Gutter()
		{
			static FName Gutter("Gutter");
			return Gutter;
		}

		/** The item label column */
		static const FName& Label()
		{
			static FName Label("ItemLabel");
			return Label;
		}

		/** Generic actor info column */
		static FName& ActorInfo()
		{
			static FName ActorInfo("ActorInfo");
			return ActorInfo;
		}
	};

	/** Visibility enum for scene outliner columns */
	enum class EColumnVisibility : uint8
	{
		/** This column defaults to being visible on the scene outliner */
		Visible,

		/** This column defaults to being invisible, yet still available on the scene outliner */
		Invisible,
	};

	/** Column information for the scene outliner */
	struct FColumnInfo
	{
		FColumnInfo(EColumnVisibility InVisibility, int32 InPriorityIndex, const FCreateSceneOutlinerColumn& InFactory = FCreateSceneOutlinerColumn())
			: Visibility(InVisibility), PriorityIndex(InPriorityIndex), Factory(InFactory)
		{}

		EColumnVisibility 	Visibility;
		uint8				PriorityIndex;

		FCreateSceneOutlinerColumn	Factory;
	};

	struct FSharedDataBase
	{
		/** Mode to operate in */
		ESceneOutlinerMode Mode;

		/**	Invoked whenever the user attempts to delete an actor from within the Scene Outliner */
		FCustomSceneOutlinerDeleteDelegate CustomDelete;

		/** Override default context menu handling */
		FOnContextMenuOpening ContextMenuOverride;

		/** Extend default context menu handling */
		TSharedPtr<FExtender> DefaultMenuExtender;

		/** Map of column types available to the scene outliner, along with default ordering */
		TMap<FName, FColumnInfo> ColumnMap;
		
		/** Whether the Scene Outliner should display parenta actors in a Tree */
		bool bShowParentTree : 1;

		/** True to only show folders in this outliner */
		bool bOnlyShowFolders : 1;

	public:

		/** Constructor */
		FSharedDataBase()
			: Mode( ESceneOutlinerMode::ActorPicker )
			, bShowParentTree( true )
			, bOnlyShowFolders( false )
		{}

		/** Set up a default array of columns for this outliner */
		void UseDefaultColumns()
		{
			if (Mode == ESceneOutlinerMode::ActorBrowsing)
			{
				ColumnMap.Add( FBuiltInColumnTypes::Gutter(), FColumnInfo(EColumnVisibility::Visible, 0) );
			}

			ColumnMap.Add( FBuiltInColumnTypes::Label(), FColumnInfo(EColumnVisibility::Visible, 10) );
			ColumnMap.Add( FBuiltInColumnTypes::ActorInfo(), FColumnInfo(EColumnVisibility::Visible, 20) );
		}
	};

	/**
	 * Settings for the Scene Outliner set by the programmer before spawning an instance of the widget.  This
	 * is used to modify the outliner's behavior in various ways, such as filtering in or out specific classes
	 * of actors.
	 */
	struct FInitializationOptions : FSharedDataBase
	{
		/** True if we should draw the header row above the tree view */
		bool bShowHeaderRow : 1;

		/** Whether the Scene Outliner should expose its searchbox */
		bool bShowSearchBox : 1;

		/** If true, the search box will gain focus when the scene outliner is created */
		bool bFocusSearchBoxWhenOpened : 1;

		/** Optional collection of filters to use when filtering in the Scene Outliner */
		TSharedPtr<FOutlinerFilters> Filters;

		/** Broadcasts whenever the Scene Outliners selection changes */
		FSimpleMulticastDelegate::FDelegate OnSelectionChanged;

	public:

		/** Constructor */
		FInitializationOptions()
			: bShowHeaderRow( true )
			, bShowSearchBox( true )
			, bFocusSearchBoxWhenOpened( false )
			, Filters( new FOutlinerFilters )
		{}
	};

	/** Outliner data that is shared between a scene outliner and its items */
	struct FSharedOutlinerData : FSharedDataBase, TSharedFromThis<FSharedOutlinerData>
	{
		/** Whether the scene outliner is currently displaying PlayWorld actors */
		bool bRepresentingPlayWorld;

		/** The world that we are representing */
		UWorld* RepresentingWorld;

		FSharedOutlinerData()
			: bRepresentingPlayWorld(false)
			, RepresentingWorld(nullptr)
		{}
	};

	/** Default metrics for outliner tree items */
	struct FDefaultTreeItemMetrics
	{
		static int32	IconSize() { return 18; };
		static FMargin	IconPadding() { return FMargin(0.f, 0.f, 6.f, 0.f); };
	};

}	// namespace SceneOutliner