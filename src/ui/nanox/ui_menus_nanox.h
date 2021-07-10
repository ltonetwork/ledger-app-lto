#ifndef __UI_MENUS_NANOX_H__
#define __UI_MENUS_NANOX_H__

#include "os.h"

#if defined(TARGET_NANOX)

#include "cx.h"
#include "ux.h"

// Helper function to display attachments
void display_if_buffer_not_empty(char* buffer, size_t buffer_len){
  if(strnlen(buffer, buffer_len) == 0){
    if (G_ux.flow_stack[0].index < G_ux.flow_stack[0].prev_index) {
      ux_flow_prev();
    }
    else if (G_ux.flow_stack[0].index > G_ux.flow_stack[0].prev_index) {
      ux_flow_next();
    }
  }
}

// Device idle flow
UX_STEP_NOCB(
    ux_idle_flow_1_step,
    pnn,
    {
      &C_icon_lto,
      "Application",
      "is ready",
    });
UX_STEP_NOCB(
    ux_idle_flow_2_step,
    bn,
    {
      "Version",
      APPVERSION,
    });
UX_STEP_NOCB(
    ux_idle_flow_3_step,
    bn,
    {
      "Developed by",
      "Stakely.io",
    });
UX_STEP_VALID(
    ux_idle_flow_4_step,
    pb,
    os_sched_exit(-1),
    {
      &C_icon_dashboard_x,
      "Quit",
    });

UX_FLOW(ux_idle_flow,
  &ux_idle_flow_1_step,
  &ux_idle_flow_2_step,
  &ux_idle_flow_3_step,
  &ux_idle_flow_4_step
);

// Verify address flow
UX_STEP_NOCB(
    ux_display_address_flow_1_step,
    pnn,
    {
      &C_icon_eye,
      "Verify",
      "address",
    });
UX_STEP_NOCB(
    ux_display_address_flow_4_step,
    bnnn_paging,
    {
      .title = "Address",
      .text = (const char *)tmp_ctx.address_context.address,
    });
UX_STEP_VALID(
    ux_display_address_flow_5_step,
    pb,
    io_seproxyhal_touch_address_ok(NULL),
    {
      &C_icon_validate_14,
      "Approve",
    });
UX_STEP_VALID(
    ux_display_address_flow_6_step,
    pb,
    io_seproxyhal_cancel(NULL),
    {
      &C_icon_crossmark,
      "Reject",
    });


UX_FLOW(ux_display_address_flow,
  &ux_display_address_flow_1_step,
  &ux_display_address_flow_4_step,
  &ux_display_address_flow_5_step,
  &ux_display_address_flow_6_step
);

// Transfer transaction flow
UX_STEP_NOCB(ux_transfer_1_step,
    pnn,
    {
      &C_icon_eye,
      "Review",
      "Transfer",
    });
UX_STEP_NOCB(
    ux_transfer_2_step,
    bnnn_paging,
    {
      .title = "Amount (LTO)",
      .text = (const char *)ui_context.line1,
    });
UX_STEP_NOCB(
    ux_transfer_3_step,
    bnnn_paging,
    {
      .title = "To",
      .text = (const char *)ui_context.line2,
    });
UX_STEP_NOCB(
    ux_transfer_4_step,
    bnnn_paging,
    {
      .title = "Fees (LTO)",
      .text = (const char *)ui_context.line3,
    });
UX_STEP_NOCB_INIT(
    ux_transfer_5_step,
    bnnn_paging,
    display_if_buffer_not_empty(ui_context.line4, sizeof(ui_context.line4)),
    {
      .title = "Attachment",
      .text = (const char *)ui_context.line4,
    });
UX_STEP_NOCB(
    ux_transfer_6_step,
    bnnn_paging,
    {
      .title = "From",
      .text = (const char *)ui_context.line5,
    });
UX_STEP_NOCB(
    ux_transfer_7_step,
    bnnn_paging,
    {
      .title = "Transaction Id",
      .text = (const char *)ui_context.line6,
    });
UX_STEP_VALID(
    ux_transfer_8_step,
    pbb,
    io_seproxyhal_touch_sign_approve(NULL),
    {
      &C_icon_validate_14,
      "Accept",
      "and send",
    });
UX_STEP_VALID(
    ux_transfer_9_step,
    pb,
    io_seproxyhal_cancel(NULL),
    {
      &C_icon_crossmark,
      "Reject",
    });

UX_FLOW(ux_transfer_flow,
  &ux_transfer_1_step,
  &ux_transfer_2_step,
  &ux_transfer_3_step,
  &ux_transfer_4_step,
  &ux_transfer_5_step,
  &ux_transfer_6_step,
  &ux_transfer_7_step,
  &ux_transfer_8_step,
  &ux_transfer_9_step
);

// Start lease transaction flow
UX_STEP_NOCB(
    ux_start_lease_1_step, 
    bnnn_paging, 
    {
      .title = "Confirm",
      .text = (const char *) ui_context.line1
    });
UX_STEP_NOCB(
    ux_start_lease_2_step, 
    bnnn_paging, 
    {
      .title = "To",
      .text = (const char *) ui_context.line2,
    });
UX_STEP_NOCB(
    ux_start_lease_3_step, 
    bnnn_paging, 
    {
      .title = "Lease Amount (LTO)",
      .text = (const char *) ui_context.line3,
    });
UX_STEP_NOCB(
    ux_start_lease_4_step, 
    bnnn_paging, 
    {
      .title = "Fees (LTO)",
      .text = (const char *) ui_context.line4,
    });
UX_STEP_NOCB(
    ux_start_lease_5_step, 
    bnnn_paging, 
    {
      .title =(const char *) ui_context.line5,
      .text = (const char *) ui_context.line6,
    });
UX_STEP_NOCB(
    ux_start_lease_6_step, 
    bnnn_paging, 
    {
      .title = "From",
      .text = (const char *) ui_context.line7,
    });
UX_STEP_VALID(
    ux_start_lease_7_step, 
    pbb, 
    io_seproxyhal_touch_sign_approve(NULL),
    {
      &C_icon_validate_14,
      "Accept",
      "and sign",
    });
UX_STEP_VALID(
    ux_start_lease_8_step, 
    pb, 
    io_seproxyhal_cancel(NULL),
    {
      &C_icon_crossmark,
      "Reject",
    });

UX_FLOW(ux_start_lease_flow,
  &ux_start_lease_1_step,
  &ux_start_lease_2_step,
  &ux_start_lease_3_step,
  &ux_start_lease_4_step,
  &ux_start_lease_5_step,
  &ux_start_lease_6_step,
  &ux_start_lease_7_step,
  &ux_start_lease_8_step
);

// Cancel lease transaction flow
UX_STEP_NOCB(
    ux_cancel_lease_1_step, 
    bnnn_paging, 
    {
      .title = "Confirm",
      .text = (const char *) ui_context.line1
    });
UX_STEP_NOCB(
    ux_cancel_lease_2_step, 
    bnnn_paging, 
    {
      .title = "Fees (LTO)",
      .text = (const char *) ui_context.line2,
    });
UX_STEP_NOCB(
    ux_cancel_lease_3_step, 
    bnnn_paging, 
    {
      .title =(const char *) ui_context.line3,
      .text = (const char *) ui_context.line4,
    });
UX_STEP_NOCB(
    ux_cancel_lease_4_step, 
    bnnn_paging, 
    {
      .title = "From",
      .text = (const char *) ui_context.line5,
    });
UX_STEP_VALID(
    ux_cancel_lease_5_step, 
    pbb, 
    io_seproxyhal_touch_sign_approve(NULL),
    {
      &C_icon_validate_14,
      "Accept",
      "and sign",
    });
UX_STEP_VALID(
    ux_cancel_lease_6_step, 
    pb, 
    io_seproxyhal_cancel(NULL),
    {
      &C_icon_crossmark,
      "Reject",
    });

UX_FLOW(ux_cancel_lease_flow,
  &ux_cancel_lease_1_step,
  &ux_cancel_lease_2_step,
  &ux_cancel_lease_3_step,
  &ux_cancel_lease_4_step,
  &ux_cancel_lease_5_step,
  &ux_cancel_lease_6_step
);

// Anchor transaction flow
UX_STEP_NOCB(
    ux_anchor_1_step, 
    bnnn_paging, 
    {
      .title = "Confirm",
      .text = (const char *) ui_context.line1
    });
UX_STEP_NOCB(
    ux_anchor_2_step, 
    bnnn_paging, 
    {
      .title = "Fees (LTO)",
      .text = (const char *) ui_context.line2,
    });
UX_STEP_NOCB(
    ux_anchor_3_step, 
    bnnn_paging, 
    {
      .title =(const char *) ui_context.line3,
      .text = (const char *) ui_context.line4,
    });
UX_STEP_NOCB(
    ux_anchor_4_step, 
    bnnn_paging, 
    {
      .title = "From",
      .text = (const char *) ui_context.line5,
    });
UX_STEP_VALID(
    ux_anchor_5_step, 
    pbb, 
    io_seproxyhal_touch_sign_approve(NULL),
    {
      &C_icon_validate_14,
      "Accept",
      "and sign",
    });
UX_STEP_VALID(
    ux_anchor_6_step, 
    pb, 
    io_seproxyhal_cancel(NULL),
    {
      &C_icon_crossmark,
      "Reject",
    });

UX_FLOW(ux_anchor_flow,
  &ux_anchor_1_step,
  &ux_anchor_2_step,
  &ux_anchor_3_step,
  &ux_anchor_4_step,
  &ux_anchor_5_step,
  &ux_anchor_6_step
);

// Generic transaction flow
UX_STEP_NOCB(
    ux_verify_transaction_1_step, 
    bnnn_paging, 
    {
      .title = "Confirm",
      .text = (const char *) ui_context.line1
    });
UX_STEP_NOCB(
    ux_verify_transaction_2_step, 
    bnnn_paging, 
    {
      .title =(const char *) ui_context.line2,
      .text = (const char *) ui_context.line3,
    });
UX_STEP_NOCB(
    ux_verify_transaction_3_step, 
    bnnn_paging, 
    {
      .title = "From",
      .text = (const char *) ui_context.line4,
    });
UX_STEP_VALID(
    ux_verify_transaction_4_step, 
    pbb, 
    io_seproxyhal_touch_sign_approve(NULL),
    {
      &C_icon_validate_14,
      "Accept",
      "and sign",
    });
UX_STEP_VALID(
    ux_verify_transaction_5_step, 
    pb, 
    io_seproxyhal_cancel(NULL),
    {
      &C_icon_crossmark,
      "Reject",
    });

UX_FLOW(ux_verify_transaction_flow,
  &ux_verify_transaction_1_step,
  &ux_verify_transaction_2_step,
  &ux_verify_transaction_3_step,
  &ux_verify_transaction_4_step,
  &ux_verify_transaction_5_step
);

#endif

#endif